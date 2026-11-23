# Statement-level control-flow reachability analysis.
#
# Original project author:: Yutaka Yanoh <mailto:yanoh@users.sourceforge.net>
# Origin::    Local metric changes recorded 2026-08-31, not the 3.2.14 baseline.
# Copyright:: Copyright (C) 2010-2014, OGIS-RI Co.,Ltd.
# License::   GPLv3+: GNU General Public License version 3 or later
#
# Maintainer:: Innoventix; Pieter Marx <mailto:pietermarx@innoventix.co.za>
# Modified::  2026-11-23: clarify provenance; see NOTICES.md and ChangeLog.

#--
#     ___    ____  __    ___   _________
#    /   |  / _  |/ /   / / | / /__  __/           Source Code Static Analyzer
#   / /| | / / / / /   / /  |/ /  / /                   AdLint - Advanced Lint
#  / __  |/ /_/ / /___/ / /|  /  / /
# /_/  |_|_____/_____/_/_/ |_/  /_/   Copyright (C) 2010-2014, OGIS-RI Co.,Ltd.
#
# This file is part of AdLint.
#
# AdLint is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# AdLint is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# AdLint.  If not, see <http://www.gnu.org/licenses/>.
#
#++

require "set"
require "adlint/cc1/syntax"

module AdLint #:nodoc:
module Cc1 #:nodoc:

  # Reconciles the abstract interpreter's path-feasibility results with the
  # exact control transfers expressed by the C statement tree.  In particular,
  # this pass handles goto targets, mixed terminating branches, loop back-edges,
  # and switch dispatch without requiring the value-domain interpreter to
  # execute source text out of lexical order.
  class StatementReachability
    def initialize(traits)
      @traits = traits
    end

    def execute(translation_unit)
      translation_unit.external_declarations.each do |external_dcl|
        if external_dcl.kind_of?(FunctionDefinition) &&
            external_dcl.analysis_target?(@traits)
          analyze_function(external_dcl)
        end
      end
    end

    private
    def analyze_function(fun_def)
      @edges = Hash.new { |hash, key| hash[key] = Set.new }
      @statements = Set.new
      @originally_executed = {}

      function_exit = Object.new
      entry = build_statement(fun_def.function_body, function_exit, nil, nil)

      @statements.each { |stmt| stmt.control_flow_reachable = false }
      visited = mark_reachable_from(entry)
      fun_def.may_fall_through = visited.include?(function_exit)
    end

    def mark_reachable_from(entry)
      worklist = entry ? [entry] : []
      visited = Set.new

      until worklist.empty?
        node = worklist.pop
        next if visited.include?(node)

        visited.add(node)
        node.control_flow_reachable = true if node.kind_of?(Statement)
        @edges[node].each { |successor| worklist.push(successor) }
      end

      visited
    end

    def register(stmt)
      unless @statements.include?(stmt)
        @statements.add(stmt)
        @originally_executed[stmt] = stmt.executed?
      end
      stmt
    end

    def add_edge(from, to)
      @edges[from].add(to) if from && to
    end

    def build_statement(stmt, follow, break_target, continue_target)
      register(stmt)

      case stmt
      when CompoundStatement
        next_entry = follow
        stmt.block_items.reverse_each do |item|
          if item.kind_of?(Statement)
            next_entry = build_statement(item, next_entry, break_target,
                                         continue_target)
          end
        end
        add_edge(stmt, next_entry)

      when GenericLabeledStatement,
           CaseLabeledStatement, DefaultLabeledStatement
        child_entry = build_statement(stmt.statement, follow, break_target,
                                      continue_target)
        add_edge(stmt, child_entry)

      when IfStatement
        then_entry = build_statement(stmt.statement, follow, break_target,
                                     continue_target)
        add_edge(stmt, then_entry) if stmt.flow_may_be_true?
        add_edge(stmt, follow) if stmt.flow_may_be_false?

      when IfElseStatement
        then_entry = build_statement(stmt.then_statement, follow, break_target,
                                     continue_target)
        else_entry = build_statement(stmt.else_statement, follow, break_target,
                                     continue_target)
        add_edge(stmt, then_entry) if stmt.flow_may_be_true?
        add_edge(stmt, else_entry) if stmt.flow_may_be_false?

      when SwitchStatement
        # Build every lexical successor in the body, but enter it only through
        # a feasible case/default label.  Unlabelled statements before the
        # first switch label therefore have no switch-entry predecessor.
        build_statement(stmt.statement, follow, follow, continue_target)
        labels = collect_switch_labels(stmt.statement)
        labels.each do |label|
          add_edge(stmt, label) if switch_label_may_be_selected?(stmt, label)
        end
        add_edge(stmt, follow) unless labels.any? { |label|
          label.kind_of?(DefaultLabeledStatement)
        }

      when WhileStatement
        body_entry = build_statement(stmt.statement, stmt, follow, stmt)
        add_edge(stmt, body_entry) if stmt.flow_may_be_true?
        add_edge(stmt, follow) if stmt.flow_may_be_false?

      when DoStatement
        condition = Object.new
        body_entry = build_statement(stmt.statement, condition, follow,
                                     condition)
        add_edge(stmt, body_entry)
        add_edge(condition, body_entry) if stmt.flow_may_be_true?
        add_edge(condition, follow) if stmt.flow_may_be_false?

      when ForStatement
        condition = register(stmt.condition_statement)
        initial = build_statement(stmt.initial_statement, condition,
                                  break_target, continue_target)
        body_entry = build_statement(stmt.body_statement, condition, follow,
                                     condition)
        add_edge(stmt, initial)
        add_edge(condition, body_entry) if stmt.flow_may_be_true?
        add_edge(condition, follow) if stmt.flow_may_be_false?

      when C99ForStatement
        condition = register(stmt.condition_statement)
        body_entry = build_statement(stmt.body_statement, condition, follow,
                                     condition)
        add_edge(stmt, condition)
        add_edge(condition, body_entry) if stmt.flow_may_be_true?
        add_edge(condition, follow) if stmt.flow_may_be_false?

      when GotoStatement
        add_edge(stmt, stmt.target)

      when ContinueStatement
        add_edge(stmt, continue_target)

      when BreakStatement
        add_edge(stmt, break_target)

      when ReturnStatement
        # A return-statement has no successor in the current function.

      else
        add_edge(stmt, follow) if stmt.flow_may_fall_through?
      end

      stmt
    end

    def switch_label_may_be_selected?(switch_stmt, label)
      if label.flow_condition_recorded?
        label.flow_may_be_true?
      elsif @originally_executed[switch_stmt]
        @originally_executed[label]
      else
        # A switch reached only through a newly resolved goto path has no
        # abstract-interpreter observation.  Conservatively retain every arm.
        true
      end
    end

    def collect_switch_labels(stmt)
      case stmt
      when SwitchStatement
        # Labels belonging to a nested switch are not dispatch targets of the
        # switch currently being collected.
        []
      when GenericLabeledStatement
        collect_switch_labels(stmt.statement)
      when CaseLabeledStatement, DefaultLabeledStatement
        [stmt] + collect_switch_labels(stmt.statement)
      when CompoundStatement
        stmt.block_items.map { |item|
          item.kind_of?(Statement) ? collect_switch_labels(item) : []
        }.flatten
      when IfStatement
        collect_switch_labels(stmt.statement)
      when IfElseStatement
        collect_switch_labels(stmt.then_statement) +
          collect_switch_labels(stmt.else_statement)
      when WhileStatement, DoStatement
        collect_switch_labels(stmt.statement)
      when ForStatement, C99ForStatement
        collect_switch_labels(stmt.body_statement)
      else
        []
      end
    end
  end

end
end
