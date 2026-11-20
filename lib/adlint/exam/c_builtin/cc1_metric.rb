# Metric measurements (cc1-phase) of adlint-exam-c_builtin package.
#
# Author::    Yutaka Yanoh <mailto:yanoh@users.sourceforge.net>
# Copyright:: Copyright (C) 2010-2014, OGIS-RI Co.,Ltd.
# Modified::  2026-11-19; see ChangeLog.
# License::   GPLv3+: GNU General Public License version 3 or later
#
# Owner::     Yutaka Yanoh <mailto:yanoh@users.sourceforge.net>

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

require "adlint/exam"
require "adlint/report"
require "adlint/cc1/phase"
require "adlint/cc1/syntax"

module AdLint #:nodoc:
module Exam #:nodoc:
module CBuiltin #:nodoc:

  class FL_STMT < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_error_statement           += T(:count_statement)
      trav.enter_generic_labeled_statement += T(:count_statement)
      trav.enter_case_labeled_statement    += T(:count_statement)
      trav.enter_default_labeled_statement += T(:count_statement)
      trav.enter_expression_statement      += T(:count_expression_statement)
      trav.enter_if_statement              += T(:count_statement)
      trav.enter_if_else_statement         += T(:count_statement)
      trav.enter_switch_statement          += T(:count_statement)
      trav.enter_while_statement           += T(:count_statement)
      trav.enter_do_statement              += T(:count_statement)
      trav.enter_for_statement             += T(:count_for_statement)
      trav.enter_c99_for_statement         += T(:count_c99_for_statement)
      trav.enter_goto_statement            += T(:count_statement)
      trav.enter_continue_statement        += T(:count_statement)
      trav.enter_break_statement           += T(:count_statement)
      trav.enter_return_statement          += T(:count_statement)
      trav.leave_translation_unit          += M(:measure)
      @stmt_cnt = 0
      @for_header_statements = {}
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def count_statement(stmt)
      if @fpath == stmt.location.fpath
        @stmt_cnt += 1
      end
    end

    def count_expression_statement(stmt)
      return if @for_header_statements.delete(stmt.object_id)
      count_statement(stmt)
    end

    def count_for_statement(stmt)
      count_statement(stmt)
      @for_header_statements[stmt.initial_statement.object_id] = true
      @for_header_statements[stmt.condition_statement.object_id] = true
    end

    def count_c99_for_statement(stmt)
      count_statement(stmt)
      @for_header_statements[stmt.condition_statement.object_id] = true
    end

    def measure(*)
      FL_STMT(@fpath, @stmt_cnt)
    end
  end

  class FL_FUNC < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:count_function)
      trav.enter_kandr_function_definition += T(:count_function)
      trav.leave_translation_unit          += M(:measure)
      @fun_cnt = 0
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def count_function(fun_def)
      if @fpath == fun_def.location.fpath
        @fun_cnt += 1
      end
    end

    def measure(*)
      FL_FUNC(@fpath, @fun_cnt)
    end
  end

  class FN_STMT < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:enter_function)
      trav.leave_ansi_function_definition  += T(:leave_function)
      trav.enter_kandr_function_definition += T(:enter_function)
      trav.leave_kandr_function_definition += T(:leave_function)
      trav.enter_error_statement           += T(:count_statement)
      trav.enter_generic_labeled_statement += T(:count_statement)
      trav.enter_case_labeled_statement    += T(:count_statement)
      trav.enter_default_labeled_statement += T(:count_statement)
      trav.enter_expression_statement      += T(:count_expression_statement)
      trav.enter_if_statement              += T(:count_statement)
      trav.enter_if_else_statement         += T(:count_statement)
      trav.enter_switch_statement          += T(:count_statement)
      trav.enter_while_statement           += T(:count_statement)
      trav.enter_do_statement              += T(:count_statement)
      trav.enter_for_statement             += T(:count_for_statement)
      trav.enter_c99_for_statement         += T(:count_c99_for_statement)
      trav.enter_goto_statement            += T(:count_statement)
      trav.enter_continue_statement        += T(:count_statement)
      trav.enter_break_statement           += T(:count_statement)
      trav.enter_return_statement          += T(:count_statement)
      @cur_fun = nil
      @stmt_cnt = 0
      @for_header_statements = {}
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def)
      @cur_fun = fun_def
      @stmt_cnt = 0
    end

    def leave_function(*)
      if @cur_fun
        FN_STMT(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, @stmt_cnt)

        @cur_fun = nil
        @stmt_cnt = 0
      end
    end

    def count_statement(*)
      @stmt_cnt += 1 if @cur_fun
    end

    def count_expression_statement(stmt)
      return if @for_header_statements.delete(stmt.object_id)
      count_statement(stmt)
    end

    def count_for_statement(stmt)
      count_statement(stmt)
      @for_header_statements[stmt.initial_statement.object_id] = true
      @for_header_statements[stmt.condition_statement.object_id] = true
    end

    def count_c99_for_statement(stmt)
      count_statement(stmt)
      @for_header_statements[stmt.condition_statement.object_id] = true
    end
  end

  class FN_UNRC < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:enter_function)
      trav.leave_ansi_function_definition  += T(:leave_function)
      trav.enter_kandr_function_definition += T(:enter_function)
      trav.leave_kandr_function_definition += T(:leave_function)
      trav.enter_error_statement           += T(:count_statement)
      trav.enter_generic_labeled_statement += T(:count_statement)
      trav.enter_case_labeled_statement    += T(:count_statement)
      trav.enter_default_labeled_statement += T(:count_statement)
      trav.enter_expression_statement      += T(:count_expression_statement)
      trav.enter_if_statement              += T(:count_statement)
      trav.enter_if_else_statement         += T(:count_statement)
      trav.enter_switch_statement          += T(:count_statement)
      trav.enter_while_statement           += T(:count_statement)
      trav.enter_do_statement              += T(:count_statement)
      trav.enter_for_statement             += T(:count_for_statement)
      trav.enter_c99_for_statement         += T(:count_c99_for_statement)
      trav.enter_goto_statement            += T(:count_statement)
      trav.enter_continue_statement        += T(:count_statement)
      trav.enter_break_statement           += T(:count_statement)
      trav.enter_return_statement          += T(:count_statement)
      @cur_fun = nil
      @unreached_stmt_cnt = 0
      @for_header_statements = {}
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def)
      @cur_fun = fun_def
      @unreached_stmt_cnt = 0
    end

    def leave_function(*)
      if @cur_fun
        FN_UNRC(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, @unreached_stmt_cnt)
        @cur_fun = nil
        @unreached_stmt_cnt = 0
      end
    end

    def count_statement(stmt)
      if @cur_fun
        @unreached_stmt_cnt += 1 unless stmt.control_flow_reachable?
      end
    end

    def count_expression_statement(stmt)
      return if @for_header_statements.delete(stmt.object_id)
      count_statement(stmt)
    end

    def count_for_statement(stmt)
      count_statement(stmt)
      @for_header_statements[stmt.initial_statement.object_id] = true
      @for_header_statements[stmt.condition_statement.object_id] = true
    end

    def count_c99_for_statement(stmt)
      count_statement(stmt)
      @for_header_statements[stmt.condition_statement.object_id] = true
    end
  end

  class FN_LINE < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:measure)
      trav.enter_kandr_function_definition += T(:measure)
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def measure(fun_def)
      FN_LINE(FunctionId.new(fun_def.identifier.value, fun_def.signature.to_s),
              fun_def.location, fun_def.lines)
    end
  end

  class FN_PARA < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:measure_ansi_function)
      trav.enter_kandr_function_definition += T(:measure_kandr_function)
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def measure_ansi_function(fun_def)
      # TODO: Determine how many parameters if function has va_list.
      if fun_def.parameter_type_list
        params = fun_def.parameter_definitions
        FN_PARA(FunctionId.new(fun_def.identifier.value,
                               fun_def.signature.to_s),
                fun_def.location, params.count { |param| !param.type.void? })
      else
        # TODO: Determine how many parameters if signature is abbreviated.
        FN_PARA(FunctionId.new(fun_def.identifier.value,
                               fun_def.signature.to_s),
                fun_def.location, 0)
      end
    end

    def measure_kandr_function(fun_def)
      FN_PARA(FunctionId.new(fun_def.identifier.value, fun_def.signature.to_s),
              fun_def.location, fun_def.identifier_list.size)
    end
  end

  class FN_UNUV < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    Write = Struct.new(:branch, :used)

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      interp = phase_ctxt[:cc1_interpreter]
      interp.on_function_started        += T(:enter_function)
      interp.on_function_ended          += T(:leave_function)
      interp.on_variable_defined        += T(:define_variable)
      interp.on_parameter_defined       += T(:define_variable)
      interp.on_variable_referred       += T(:refer_variable)
      interp.on_variable_value_referred += T(:read_variable)
      interp.on_variable_value_updated  += T(:write_variable)
      interp.on_branch_started         += M(:enter_usage_branch)
      interp.on_branch_ended            += M(:merge_finished_branch)
      interp.on_goto_stmt_evaled        += T(:jump_usage_path)
      interp.on_label_defined           += T(:join_usage_label)
      [:while, :do, :for, :c99_for, :switch].each do |kind|
        start_event = "on_#{kind}_stmt_started"
        end_event = "on_#{kind}_stmt_ended"
        interp.__send__("#{start_event}=",
                        interp.__send__(start_event) + T(:enter_usage_control))
        interp.__send__("#{end_event}=",
                        interp.__send__(end_event) + T(:leave_usage_control))
      end
      @cur_fun = nil
      @variables = nil
      @interpreter = interp
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def, *)
      @cur_fun = fun_def
      @variables = {}
      @loop_usage = LoopValueUsage.new
    end

    def leave_function(*)
      if @cur_fun
        @loop_usage.mark_loop_reads
        useless_cnt = @variables.each_value.reduce(0) { |cnt, variable|
          if variable[:writes].empty?
            variable[:referenced] ? cnt : cnt + 1
          else
            cnt + variable[:writes].count { |write| !write.used }
          end
        }

        FN_UNUV(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, useless_cnt)
        @cur_fun = nil
        @variables = nil
        @loop_usage = nil
      end
    end

    def define_variable(*, var)
      @variables[variable_key(var)] = { :referenced => false, :writes => [],
                                        :live_writes => [] } if
        @cur_fun && var.named?
      if @cur_fun && var.named? && !var.declared_as_static?
        @loop_usage.access(:write, variable_key(var))
      end
    end

    def refer_variable(*, var)
      variable = tracked_variable(var)
      variable[:referenced] = true if variable
    end

    def read_variable(expr, var)
      @loop_usage.observe_expression(expr) if @loop_usage
      variable = tracked_variable(var)
      return unless variable

      variable[:referenced] = true
      @loop_usage.access(:read, variable_key(var))
      variable[:live_writes].each do |write|
        write.used = true if write_reaches_current_branch?(write)
      end
    end

    def write_variable(expr, var)
      @loop_usage.observe_expression(expr) if @loop_usage
      variable = tracked_variable(var)
      return unless variable

      variable[:live_writes].reject! do |write|
        overwritten_on_every_path_to_current_branch?(write)
      end

      write = Write.new(@interpreter.current_branch, false)
      # The interpreter suppresses some read notifications for ++, -- and
      # compound assignment to avoid duplicate sequence-point warnings.
      if expr.kind_of?(Cc1::CompoundAssignmentExpression) ||
          expr.kind_of?(Cc1::PrefixIncrementExpression) ||
          expr.kind_of?(Cc1::PostfixIncrementExpression) ||
          expr.kind_of?(Cc1::PrefixDecrementExpression) ||
          expr.kind_of?(Cc1::PostfixDecrementExpression)
        @loop_usage.access(:read, variable_key(var))
      end
      @loop_usage.access(:write, variable_key(var), write)
      variable[:writes].push(write)
      variable[:live_writes].push(write)
    end

    def merge_finished_branch(branch)
      @loop_usage.leave_branch(branch) if @loop_usage
      return unless @variables && branch.final?

      @variables.each_value do |variable|
        variable[:live_writes].reject! do |write|
          write.branch && write.branch.group == branch.group &&
            write.branch.break_with_return?
        end

        variable[:live_writes].each do |write|
          if write.branch && write.branch.group == branch.group
            write.branch = branch.trunk
          end
        end
      end
    end

    def tracked_variable(var)
      @variables[variable_key(var)] if @variables && var.named?
    end

    def enter_usage_branch(branch)
      @loop_usage.enter_branch(branch) if @loop_usage
    end

    def enter_usage_control(stmt)
      @loop_usage.enter_control(stmt) if @loop_usage
    end

    def leave_usage_control(stmt)
      @loop_usage.leave_control(stmt) if @loop_usage
    end

    def jump_usage_path(stmt, *)
      @loop_usage.jump(stmt.target) if @loop_usage
    end

    def join_usage_label(stmt)
      @loop_usage.label(stmt) if @loop_usage
    end

    def variable_key(var)
      # The interpreter creates separate value versions for the same source
      # variable in different control-flow branches. Its declaration or
      # definition remains the stable scope-aware identity across versions.
      var.declarations_and_definitions.first
    end

    def write_reaches_current_branch?(write)
      branch_in_current_path?(write.branch)
    end

    def overwritten_on_every_path_to_current_branch?(write)
      # A write outside a branch follows every surviving branch path.  Inside
      # a branch, only an earlier write in that same branch is certainly
      # overwritten; writes from outer or sibling branches remain possible.
      @interpreter.current_branch.nil? ||
        write.branch == @interpreter.current_branch
    end

    def branch_in_current_path?(branch)
      current_branch = @interpreter.current_branch
      return true unless current_branch
      return true unless branch

      while current_branch
        return true if current_branch == branch
        current_branch = current_branch.trunk
      end

      false
    end

    # Replay only the metric's observed accesses, without executing the C
    # interpreter again. Back-edges expose reads on later loop iterations;
    # writes and automatic-variable definitions kill the preceding value.
    # Existing non-loop counts are retained. Reconcile unread writes recorded
    # inside loops, including condition-evaluation branches, and writes read
    # after crossing a loop back-edge.
    class LoopValueUsage
      def initialize
        @events = []
        @edges = Hash.new { |hash, key| hash[key] = [] }
        @frontier = [event(:entry)]
        @groups = {}
        @controls = []
        @writes = []
        @keys = Hash.new { |hash, name| hash[name] = Set.new }
        @labels = {}
        @jumps = Hash.new { |hash, key| hash[key] = [] }
      end

      def access(kind, key, write = nil)
        @keys[key.identifier.value].add(key) if key
        node = append(kind, key)
        @writes.push([node, key, write,
                      @controls.any? { |control| control[:loop] }]) if write
      end

      def observe_expression(expr)
        @controls.each do |control|
          if !control[:condition] && control[:condition_nodes].include?(expr)
            control[:condition] = append(:entry)
            control[:entry] ||= control[:condition]
          end
          if !control[:increment] && control[:increment_nodes].include?(expr)
            control[:increment] = append(:entry)
          end
        end
      end

      def jump(target)
        if @labels[target]
          connect(@frontier, @labels[target], false)
        else
          @jumps[target].concat(@frontier)
        end
        @frontier = []
      end

      def label(stmt)
        node = append(:entry)
        @labels[stmt] = node
        connect(@jumps.delete(stmt) || [], node, false)
      end

      def enter_branch(branch)
        if branch.group.iteration?
          control = @controls.reverse.find { |item| item[:loop] }
          # for(;;), or a constant condition, may emit no condition accesses.
          # Its back-edge starts at the body, after the one-time initializer.
          control[:entry] ||= append(:entry) if control
        end
        group = @groups[branch.group] ||=
          { :entry => @frontier.dup, :exits => [] }
        @frontier = group[:entry].dup
      end

      def leave_branch(branch)
        group = @groups[branch.group]
        return unless group

        if branch.break_with_return?
          @frontier = []
        elsif branch.break_with_continue?
          control = @controls.reverse.find { |item| item[:loop] }
          control[:continues].concat(@frontier) if control
          @frontier = []
        elsif branch.break_with_break?
          @controls.last[:breaks].concat(@frontier) unless @controls.empty?
          @frontier = []
        end
        group[:exits].concat(@frontier)
        if branch.final?
          @frontier = group[:exits]
          @frontier += group[:entry] unless branch.group.complete?
          @groups.delete(branch.group)
        else
          @frontier = group[:entry].dup
        end
      end

      def enter_control(stmt)
        is_for = stmt.kind_of?(Cc1::ForStatement) ||
          stmt.kind_of?(Cc1::C99ForStatement)
        is_loop = !stmt.kind_of?(Cc1::SwitchStatement)
        condition = is_for ? stmt.condition_statement.expression : stmt.expression
        @controls.push({ :statement => stmt, :loop => is_loop,
          :entry => (is_loop && !is_for ? append(:entry) : nil),
          :condition => nil, :condition_nodes => expression_nodes(condition),
          :increment => nil,
          :increment_nodes => expression_nodes(is_for ? stmt.expression : nil),
          :has_increment => is_for && stmt.expression,
          :breaks => [], :continues => [] })
      end

      def leave_control(stmt)
        # A continue/return can bypass the switch-ended notifier. Unwind such
        # inner controls when the enclosing loop's ensure block is notified.
        index = @controls.rindex { |item| item[:statement].equal?(stmt) }
        return unless index
        control = @controls[index]
        @controls.slice!(index..-1)
        if control[:loop]
          entry = control[:entry]
          if entry && stmt.flow_may_be_true?
            connect(@frontier, entry, true)
          end
          target = if stmt.kind_of?(Cc1::DoStatement)
            control[:condition]
          elsif control[:has_increment]
            control[:increment]
          else
            entry
          end
          if target
            connect(control[:continues], target, true)
          elsif entry && control[:has_increment]
            # The interpreter skips increment evaluation when every body
            # path continues. Model simple accesses without executing it.
            saved_frontier = @frontier
            increment = event(:entry)
            connect(control[:continues], increment, true)
            @frontier = [increment]
            append_increment(stmt.expression)
            connect(@frontier, entry, false)
            @frontier = saved_frontier
          end
        end
        @frontier += control[:breaks]
      end

      def mark_loop_reads
        @writes.each do |node, key, write, inside_loop|
          next if write.used
          pending = @edges[node].dup
          visited = {}
          until pending.empty?
            current, crossed_loop = pending.pop
            state = [current, crossed_loop]
            next if visited[state]
            visited[state] = true
            kind, variable = @events[current]
            next if kind == :unknown_write &&
              (!variable || variable.include?(key.identifier.value))
            if variable.equal?(key)
              if kind == :read && (crossed_loop || inside_loop)
                write.used = true
                break
              end
              next if kind == :write
            end
            @edges[current].each do |successor, back_edge|
              pending.push([successor, crossed_loop || back_edge])
            end
          end
        end
      end

      private
      def event(kind, key = nil)
        @events.push([kind, key])
        @events.size - 1
      end

      def append(kind, key = nil)
        node = event(kind, key)
        connect(@frontier, node, false)
        @frontier = [node]
        node
      end

      def connect(predecessors, target, back_edge)
        predecessors.each { |node| @edges[node].push([target, back_edge]) }
      end

      def expression_nodes(expr)
        visitor = ExpressionNodes.new
        expr.accept(visitor) if expr
        visitor.nodes
      end

      def increment_variable(expr, kind)
        if expr.kind_of?(Cc1::ObjectSpecifier)
          keys = @keys[expr.identifier.value]
          if keys.size == 1
            append(kind, keys.first)
            return
          end
        end
        # Ambiguous scope or an indirect store: do not prove the old value
        # live through a write whose target cannot be identified safely.
        append(:unknown_write) if kind == :write
      end

      def append_increment(expr)
        case expr
        when Cc1::ObjectSpecifier
          increment_variable(expr, :read)
        when Cc1::ConstantSpecifier, Cc1::NullConstantSpecifier,
             Cc1::StringLiteralSpecifier
          # No local-variable access.
        when Cc1::GroupedExpression
          append_increment(expr.expression)
        when Cc1::CommaSeparatedExpression
          expr.expressions.each { |part| append_increment(part) }
        when Cc1::SimpleAssignmentExpression
          append_increment(expr.rhs_operand)
          increment_variable(expr.lhs_operand, :write)
        when Cc1::CompoundAssignmentExpression
          append_increment(expr.lhs_operand)
          append_increment(expr.rhs_operand)
          increment_variable(expr.lhs_operand, :write)
        when Cc1::PrefixIncrementExpression, Cc1::PostfixIncrementExpression,
             Cc1::PrefixDecrementExpression, Cc1::PostfixDecrementExpression
          append_increment(expr.operand)
          increment_variable(expr.operand, :write)
        when Cc1::FunctionCallExpression
          append_increment(expr.expression)
          expr.argument_expressions.each { |arg| append_increment(arg) }
          append(:unknown_write)
        when Cc1::LogicalAndExpression, Cc1::LogicalOrExpression,
             Cc1::ConditionalExpression
          # Retain uncertainty about conditional evaluation and side effects.
          append(:unknown_write)
        else
          if expr.respond_to?(:lhs_operand) && expr.respond_to?(:rhs_operand)
            append_increment(expr.lhs_operand)
            append_increment(expr.rhs_operand)
          else
            append(:unknown_write)
          end
        end
      end

      class ExpressionNodes < Cc1::SyntaxTreeMulticastVisitor
        def initialize
          @nodes = Set.new
        end
        attr_reader :nodes

        private
        def visit_with_notifying(_, node)
          @nodes.add(node)
          yield
        end
      end
      private_constant :ExpressionNodes
    end
    private_constant :LoopValueUsage
  end

  class FN_CSUB < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:enter_function)
      trav.leave_ansi_function_definition  += T(:leave_function)
      trav.enter_kandr_function_definition += T(:enter_function)
      trav.leave_kandr_function_definition += T(:leave_function)
      trav.enter_function_call_expression  += T(:count_function_call)
      @cur_fun = nil
      @funcall_cnt = 0
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def)
      @cur_fun = fun_def
      @funcall_cnt = 0
    end

    def leave_function(*)
      if @cur_fun
        FN_CSUB(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, @funcall_cnt)
        @cur_fun = nil
        @funcall_cnt = 0
      end
    end

    def count_function_call(*)
      if @cur_fun
        @funcall_cnt += 1
      end
    end
  end

  class FN_GOTO < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:enter_function)
      trav.leave_ansi_function_definition  += T(:leave_function)
      trav.enter_kandr_function_definition += T(:enter_function)
      trav.leave_kandr_function_definition += T(:leave_function)
      trav.enter_goto_statement            += T(:count_goto)
      @cur_fun = nil
      @goto_cnt = 0
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def)
      @cur_fun = fun_def
      @goto_cnt = 0
    end

    def leave_function(*)
      if @cur_fun
        FN_GOTO(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, @goto_cnt)
        @cur_fun = nil
        @goto_cnt = 0
      end
    end

    def count_goto(*)
      if @cur_fun
        @goto_cnt += 1
      end
    end
  end

  class FN_RETN < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:enter_function)
      trav.leave_ansi_function_definition  += T(:leave_function)
      trav.enter_kandr_function_definition += T(:enter_function)
      trav.leave_kandr_function_definition += T(:leave_function)
      trav.enter_return_statement          += T(:count_return)
      @cur_fun = nil
      @ret_cnt = 0
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def)
      @cur_fun = fun_def
      @ret_cnt = 0
    end

    def leave_function(*)
      if @cur_fun
        implicit_return = @cur_fun.type.return_type.void? &&
          @cur_fun.may_fall_through?
        FN_RETN(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, @ret_cnt + (implicit_return ? 1 : 0))
        @cur_fun = nil
        @ret_cnt = 0
      end
    end

    def count_return(node)
      if @cur_fun
        @ret_cnt += 1
      end
    end
  end

  class FN_UELS < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:enter_function)
      trav.leave_ansi_function_definition  += T(:leave_function)
      trav.enter_kandr_function_definition += T(:enter_function)
      trav.leave_kandr_function_definition += T(:leave_function)
      trav.enter_if_else_statement         += T(:enter_if_else_statement)
      trav.leave_if_else_statement         += T(:leave_if_else_statement)
      @cur_fun = nil
      @if_else_stmt_chain = 0
      @incomplete_if_else_stmt_cnt = 0
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def)
      @cur_fun = fun_def
      @if_else_stmt_chain = 0
      @incomplete_if_else_stmt_cnt = 0
    end

    def leave_function(*)
      if @cur_fun
        FN_UELS(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, @incomplete_if_else_stmt_cnt)
        @cur_fun = nil
        @if_else_stmt_chain = 0
        @incomplete_if_else_stmt_cnt = 0
      end
    end

    def enter_if_else_statement(node)
      @if_else_stmt_chain += 1

      if @cur_fun && @if_else_stmt_chain > 0
        if node.else_statement.kind_of?(Cc1::IfStatement)
          @incomplete_if_else_stmt_cnt += 1
        end
      end
    end

    def leave_if_else_statement(*)
      @if_else_stmt_chain -= 1
    end
  end

  class FN_NEST < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:enter_function)
      trav.leave_ansi_function_definition  += T(:leave_function)
      trav.enter_kandr_function_definition += T(:enter_function)
      trav.leave_kandr_function_definition += T(:leave_function)
      trav.enter_if_statement              += T(:enter_control)
      trav.leave_if_statement              += T(:leave_control)
      trav.enter_if_else_statement         += T(:enter_control)
      trav.leave_if_else_statement         += T(:leave_control)
      trav.enter_switch_statement          += T(:enter_control)
      trav.leave_switch_statement          += T(:leave_control)
      trav.enter_while_statement           += T(:enter_control)
      trav.leave_while_statement           += T(:leave_control)
      trav.enter_do_statement              += T(:enter_control)
      trav.leave_do_statement              += T(:leave_control)
      trav.enter_for_statement             += T(:enter_control)
      trav.leave_for_statement             += T(:leave_control)
      trav.enter_c99_for_statement         += T(:enter_control)
      trav.leave_c99_for_statement         += T(:leave_control)
      @cur_fun = nil
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def)
      @cur_fun = fun_def
      # NOTE: The function body's top-level statements have nesting level 0.
      @max_nest_level = @cur_nest_level = 0
      @control_stack = []
    end

    def leave_function(fun_def)
      if @cur_fun
        FN_NEST(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, @max_nest_level)
        @cur_fun = nil
        @control_stack = nil
      end
    end

    def enter_control(stmt)
      if @cur_fun
        adds_level = !else_if_arm?(stmt)
        @control_stack.push([stmt, adds_level])
        if adds_level
          @cur_nest_level += 1
          @max_nest_level = [@max_nest_level, @cur_nest_level].max
        end
      end
    end

    def leave_control(*)
      if @cur_fun
        _, adds_level = @control_stack.pop
        @cur_nest_level -= 1 if adds_level
      end
    end

    def else_if_arm?(stmt)
      parent = @control_stack.last
      # An if/else-if/else ladder is one control structure for FN_NEST.
      parent && parent[0].kind_of?(Cc1::IfElseStatement) &&
        parent[0].else_statement.equal?(stmt) &&
        (stmt.kind_of?(Cc1::IfStatement) ||
         stmt.kind_of?(Cc1::IfElseStatement))
    end
  end

  class FN_PATH < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      interp = phase_ctxt[:cc1_interpreter]
      interp.on_function_started += T(:enter_function)
      interp.on_function_ended   += T(:leave_function)
      interp.on_branch_started   += M(:enter_branch)
      interp.on_branch_ended     += M(:leave_branch)
      @cur_fun = nil
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def, *)
      @cur_fun = fun_def
      # NOTE: Number of paths in the current function.
      @paths_in_fun = 1
      # NOTE: Stack of the number of paths to enter the current branch group.
      @paths_to_enter_branch_group = [@paths_in_fun]
      # NOTE: Stack of the number of paths in the current branch.
      @paths_in_branch = [@paths_in_fun]
      # NOTE: Stack of the number of paths in the current branch group.
      @paths_in_branch_group = [@paths_in_fun]
    end

    def leave_function(*)
      if @cur_fun
        FN_PATH(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, @paths_in_fun)
        @cur_fun = nil
      end
    end

    def enter_branch(branch)
      if @cur_fun
        # NOTE: Entering into new branch group.
        if branch.first?
          @paths_in_fun -= @paths_in_branch.last
          @paths_to_enter_branch_group.push(@paths_in_branch.last)
          @paths_in_branch_group.push(0)
        end

        # NOTE: Entering into new branch.
        @paths_in_branch.push(@paths_to_enter_branch_group.last)
        @paths_in_fun += @paths_to_enter_branch_group.last
      end
    end

    def leave_branch(branch)
      if @cur_fun
        paths_in_this_branch = @paths_in_branch.pop

        # NOTE: Leaving from the current branch whose paths are not terminated.
        unless branch.break_with_return?
          @paths_in_branch_group[-1] += paths_in_this_branch
        end

        # NOTE: Leaving from the current branch group.
        if branch.final?
          paths_to_enter_this_branch_group = @paths_to_enter_branch_group.pop
          paths_in_this_branch_group = @paths_in_branch_group.pop

          @paths_in_branch[-1] = paths_in_this_branch_group

          # NOTE: The current branch group is an incomplete branch group.
          unless branch.group.complete?
            @paths_in_fun += paths_to_enter_this_branch_group
            @paths_in_branch[-1] += paths_to_enter_this_branch_group
          end
        end
      end
    end
  end

  class FN_CYCM < MetricMeasurement
    def_registrant_phase Cc1::Prepare2Phase

    def initialize(phase_ctxt)
      super
      @fpath = phase_ctxt[:sources].first.fpath
      trav = phase_ctxt[:cc1_ast_traversal]
      trav.enter_ansi_function_definition  += T(:enter_function)
      trav.leave_ansi_function_definition  += T(:leave_function)
      trav.enter_kandr_function_definition += T(:enter_function)
      trav.leave_kandr_function_definition += T(:leave_function)
      trav.enter_if_statement              += T(:enter_selection)
      trav.enter_if_else_statement         += T(:enter_selection)
      trav.enter_case_labeled_statement    += T(:enter_selection)
      # Add Loops
      trav.enter_while_statement           += T(:enter_selection)
      trav.enter_do_statement              += T(:enter_selection)
      trav.enter_for_statement             += T(:enter_selection)
      trav.enter_c99_for_statement         += T(:enter_selection)
      # Add Logical Operators
      trav.enter_logical_and_expression    += T(:enter_selection)
      trav.enter_logical_or_expression     += T(:enter_selection)
      # Add Ternary
      trav.enter_conditional_expression    += T(:enter_selection)
      @cur_fun = nil
      @cycl_compl = 0
    end

    private
    def do_prepare(*) end
    def do_execute(*) end

    def enter_function(fun_def)
      @cur_fun = fun_def
      @cycl_compl = 0
    end

    def leave_function(*)
      if @cur_fun
        FN_CYCM(FunctionId.new(@cur_fun.identifier.value,
                               @cur_fun.signature.to_s),
                @cur_fun.location, @cycl_compl + 1)
        @cur_fun = nil
      end
    end

    def enter_selection(*)
      @cycl_compl += 1 if @cur_fun
    end
  end

end
end
end
