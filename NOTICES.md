# Attribution and source provenance

This distribution is the Innoventix maintained fork of AdLint, based on the
3.2.14 release (`v3_2_14`, baseline commit `ab61a3d`). The original project
author is Yutaka Yanoh. The original OGIS-RI Co.,Ltd. copyright notices are
retained. Original authorship does not imply endorsement of this fork.

The imported local metric changes were supplied in a folder called
`adlint-3.2.15`, whose change record was dated 31 August 2026. That local
folder name does not identify an upstream release. It supplied the new
statement-reachability module, revised metrics, guide edits and public C
examples. The alternate supplied `adlint-3.2.15_Questionable` folder contained
identical shared source files; no separate source fix was imported from it.

Subsequent fork maintenance includes loop value-use corrections, automated
fixture verification, interpreter transfer/inference fixes and logical-
negation boundary corrections. Dated details and contributors are recorded
in [ChangeLog](ChangeLog). Maintainer: Innoventix; contact Pieter Marx,
<pietermarx@innoventix.co.za>. Maintainer attribution does not replace the
original notices or establish sole authorship of the supplied changes.

The later historical 3.2.15, 3.2.17 and 3.2.19 release candidates and the
outstanding third-party invalid-UTF-8 contribution have not been integrated
as change sets. Separate review and attribution would be needed before
including them. This fork does not claim to contain all subsequent fixes.

AdLint is distributed under GNU GPL version 3 or, at your option, any later
version, as stated in [COPYING](COPYING) and the retained source notices.
Keep that licence and the authorship, copyright and modification notices
with redistributed source. This provenance record does not transfer copyright
or assign ownership of newly contributed work. Copyright ownership of new
contributions has not been independently established by this technical audit.

Version 3.4.0 and the reporting date 2026-11-19 were selected for qualification
preparation. They do not establish that a public release has occurred. Builds
made before and after later source corrections may report the same version;
the exact revision and package SHA-256 distinguish them. The private wrapper
and its documentation are not part of this distribution.
