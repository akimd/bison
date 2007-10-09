<?xml version="1.0" encoding="UTF-8"?>

<!--
    xml2text.xsl - transform Bison XML Report into plain text.
    $Id$

    Copyright (C) 2007 Free Software Foundation, Inc.

    This file is part of Bison, the GNU Compiler Compiler.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Written by Wojciech Polak <polak@gnu.org>.
  -->

<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:bison="http://www.gnu.org/software/bison/">

<xsl:import href="bison.xsl"/>
<xsl:output method="text" encoding="UTF-8" indent="no"/>

<xsl:template match="/">
  <xsl:apply-templates select="bison-xml-report"/>
</xsl:template>

<xsl:template match="bison-xml-report">
  <xsl:apply-templates select="reductions"/>
  <xsl:apply-templates select="rules-never-reduced"/>
  <xsl:apply-templates select="automaton" mode="conflicts"/>
  <xsl:apply-templates select="grammar"/>
  <xsl:apply-templates select="automaton"/>
</xsl:template>

<xsl:template match="rules-never-reduced">
  <xsl:if test="rule">
    <xsl:text>Rules never reduced&#10;</xsl:text>
    <xsl:apply-templates select="rule">
      <xsl:with-param name="pad" select="'3'"/>
    </xsl:apply-templates>
    <xsl:text>&#10;&#10;</xsl:text>
  </xsl:if>
</xsl:template>

<xsl:template match="reductions">
  <xsl:apply-templates select="useless/nonterminals"/>
  <xsl:apply-templates select="unused/terminals"/>
  <xsl:apply-templates select="useless/rules"/>
</xsl:template>

<xsl:template match="useless/nonterminals">
  <xsl:if test="nonterminal">
    <xsl:text>Useless nonterminals&#10;&#10;</xsl:text>
    <xsl:for-each select="nonterminal">
      <xsl:text>   </xsl:text>
      <xsl:value-of select="."/>
      <xsl:text>&#10;</xsl:text>
    </xsl:for-each>
    <xsl:text>&#10;&#10;</xsl:text>
  </xsl:if>
</xsl:template>

<xsl:template match="useless/rules">
  <xsl:if test="rule">
    <xsl:text>Useless rules&#10;</xsl:text>
    <xsl:apply-templates select="rule">
      <xsl:with-param name="pad" select="'3'"/>
    </xsl:apply-templates>
    <xsl:text>&#10;&#10;</xsl:text>
  </xsl:if>
</xsl:template>

<xsl:template match="unused/terminals">
  <xsl:if test="terminal">
    <xsl:text>Terminals which are not used&#10;&#10;</xsl:text>
    <xsl:for-each select="terminal">
      <xsl:text>   </xsl:text>
      <xsl:value-of select="."/>
      <xsl:text>&#10;</xsl:text>
    </xsl:for-each>
    <xsl:text>&#10;&#10;</xsl:text>
  </xsl:if>
</xsl:template>

<xsl:template match="grammar">
  <xsl:text>Grammar&#10;</xsl:text>
  <xsl:apply-templates select="rules/rule">
    <xsl:with-param name="pad" select="'3'"/>
  </xsl:apply-templates>
  <xsl:text>&#10;&#10;</xsl:text>
  <xsl:apply-templates select="terminals"/>
  <xsl:apply-templates select="nonterminals"/>
</xsl:template>

<xsl:template match="grammar/terminals">
  <xsl:text>Terminals, with rules where they appear&#10;&#10;</xsl:text>
  <xsl:apply-templates select="terminal"/>
  <xsl:text>&#10;&#10;</xsl:text>
</xsl:template>

<xsl:template match="grammar/nonterminals">
  <xsl:text>Nonterminals, with rules where they appear&#10;&#10;</xsl:text>
  <xsl:apply-templates select="nonterminal"/>
</xsl:template>

<xsl:template match="terminal">
  <xsl:value-of select="@symbol"/>
  <xsl:call-template name="line-wrap">
    <xsl:with-param
      name="first-line-length" select="66 - string-length(@symbol)"
    />
    <xsl:with-param name="line-length" select="66" />
    <xsl:with-param name="text">
      <xsl:value-of select="concat(' (', @type, ')')"/>
      <xsl:apply-templates select="rule" />
    </xsl:with-param>
  </xsl:call-template>
</xsl:template>

<xsl:template match="terminal/rule">
  <xsl:text> </xsl:text>
  <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="nonterminal">
  <xsl:value-of select="@symbol"/>
  <xsl:value-of select="concat(' (', @type, ')')"/>
  <xsl:text>&#10;</xsl:text>
  <xsl:variable name="output">
    <xsl:call-template name="line-wrap">
      <xsl:with-param name="line-length" select="66" />
      <xsl:with-param name="text">
        <xsl:text>    </xsl:text>
        <xsl:if test="left/rule">
          <xsl:text>on@left:</xsl:text>
        </xsl:if>
        <xsl:apply-templates select="left/rule"/>
        <xsl:if test="left/rule and right/rule">
          <xsl:text>, </xsl:text>
        </xsl:if>
        <xsl:if test="right/rule">
          <xsl:text>on@right:</xsl:text>
        </xsl:if>
        <xsl:apply-templates select="right/rule"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:variable>
  <xsl:value-of select="translate($output, '@', ' ')" />
</xsl:template>

<xsl:template match="nonterminal/left/rule|nonterminal/right/rule">
  <xsl:text> </xsl:text>
  <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="automaton" mode="conflicts">
  <xsl:variable name="conflict-report">
    <xsl:apply-templates select="state" mode="conflicts"/>
  </xsl:variable>
  <xsl:if test="string-length($conflict-report) != 0">
    <xsl:value-of select="$conflict-report"/>
    <xsl:text>&#10;&#10;</xsl:text>
  </xsl:if>
</xsl:template>

<xsl:template match="state" mode="conflicts">
  <xsl:variable name="conflict-counts">
    <xsl:apply-templates select="." mode="bison:count-conflicts" />
  </xsl:variable>
  <xsl:variable
    name="sr-count" select="substring-before($conflict-counts, ',')"
  />
  <xsl:variable
    name="rr-count" select="substring-after($conflict-counts, ',')"
  />
  <xsl:if test="$sr-count > 0 or $rr-count > 0">
    <xsl:value-of select="concat('State ', @number, ' conflicts:')"/>
    <xsl:if test="$sr-count > 0">
      <xsl:value-of select="concat(' ', $sr-count, ' shift/reduce')"/>
      <xsl:if test="$rr-count > 0">
        <xsl:value-of select="(',')"/>
      </xsl:if>
    </xsl:if>
    <xsl:if test="$rr-count > 0">
      <xsl:value-of select="concat(' ', $rr-count, ' reduce/reduce')"/>
    </xsl:if>
    <xsl:value-of select="'&#10;'"/>
  </xsl:if>
</xsl:template>

<xsl:template match="automaton">
  <xsl:apply-templates select="state">
    <xsl:with-param name="pad" select="'3'"/>
  </xsl:apply-templates>
</xsl:template>

<xsl:template match="automaton/state">
  <xsl:param name="pad"/>
  <xsl:text>&#10;&#10;</xsl:text>
  <xsl:text>state </xsl:text>
  <xsl:value-of select="@number"/>
  <xsl:text>&#10;&#10;</xsl:text>
  <xsl:apply-templates select="itemset/rule">
    <xsl:with-param name="pad" select="$pad"/>
  </xsl:apply-templates>
  <xsl:apply-templates select="actions/transitions">
    <xsl:with-param name="type" select="'shift'"/>
  </xsl:apply-templates>
  <xsl:apply-templates select="actions/errors"/>
  <xsl:apply-templates select="actions/reductions"/>
  <xsl:apply-templates select="actions/transitions">
    <xsl:with-param name="type" select="'goto'"/>
  </xsl:apply-templates>
  <xsl:apply-templates select="solved-conflicts"/>
</xsl:template>

<xsl:template match="actions/transitions">
  <xsl:param name="type"/>
  <xsl:if test="transition[@type = $type]">
    <xsl:text>&#10;</xsl:text>
    <xsl:apply-templates select="transition[@type = $type]">
      <xsl:with-param name="pad">
	<xsl:call-template name="max-width-symbol">
	  <xsl:with-param name="node" select="transition[@type = $type]"/>
	</xsl:call-template>
      </xsl:with-param>
    </xsl:apply-templates>
  </xsl:if>
</xsl:template>

<xsl:template match="actions/errors">
  <xsl:if test="error">
    <xsl:text>&#10;</xsl:text>
    <xsl:apply-templates select="error">
      <xsl:with-param name="pad">
	<xsl:call-template name="max-width-symbol">
	  <xsl:with-param name="node" select="error"/>
	</xsl:call-template>
      </xsl:with-param>
    </xsl:apply-templates>
  </xsl:if>
</xsl:template>

<xsl:template match="actions/reductions">
  <xsl:if test="reduction">
    <xsl:text>&#10;</xsl:text>
    <xsl:apply-templates select="reduction">
      <xsl:with-param name="pad">
	<xsl:call-template name="max-width-symbol">
	  <xsl:with-param name="node" select="reduction"/>
	</xsl:call-template>
      </xsl:with-param>
    </xsl:apply-templates>
  </xsl:if>
</xsl:template>

<xsl:template match="rule">
  <xsl:param name="pad"/>
  <xsl:if test="not(name(..) = 'itemset') and not(preceding-sibling::rule[1]/lhs[text()] = lhs[text()])">
    <xsl:text>&#10;</xsl:text>
  </xsl:if>
  <xsl:text>  </xsl:text>
  <xsl:call-template name="lpad">
    <xsl:with-param name="str" select="string(@number)"/>
    <xsl:with-param name="pad" select="number($pad)"/>
  </xsl:call-template>
  <xsl:text> </xsl:text>
  <xsl:choose>
    <xsl:when test="preceding-sibling::rule[1]/lhs[text()] = lhs[text()]">
      <xsl:call-template name="lpad">
	<xsl:with-param name="str" select="'|'"/>
	<xsl:with-param name="pad" select="number(string-length(lhs[text()])) + 1"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="lhs"/>
      <xsl:text>:</xsl:text>
    </xsl:otherwise>
  </xsl:choose>
  <xsl:apply-templates select="rhs/symbol|rhs/point|rhs/empty"/>
  <xsl:apply-templates select="lookaheads"/>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="symbol">
  <xsl:text> </xsl:text>
  <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="point">
  <xsl:text> .</xsl:text>
</xsl:template>

<xsl:template match="empty">
  <xsl:text> /* empty */</xsl:text>
</xsl:template>

<xsl:template match="lookaheads">
  <xsl:text>  [</xsl:text>
  <xsl:apply-templates select="symbol"/>
  <xsl:text>]</xsl:text>
</xsl:template>

<xsl:template match="lookaheads/symbol">
  <xsl:value-of select="."/>
  <xsl:if test="position() != last()">
    <xsl:text>, </xsl:text>
  </xsl:if>
</xsl:template>

<xsl:template match="transition">
  <xsl:param name="pad"/>
  <xsl:text>    </xsl:text>
  <xsl:call-template name="rpad">
    <xsl:with-param name="str" select="string(@symbol)"/>
    <xsl:with-param name="pad" select="number($pad) + 2"/>
  </xsl:call-template>
  <xsl:choose>
    <xsl:when test="@type = 'shift'">
      <xsl:text>shift, and go to state </xsl:text>
      <xsl:value-of select="@state"/>
    </xsl:when>
    <xsl:when test="@type = 'goto'">
      <xsl:text>go to state </xsl:text>
      <xsl:value-of select="@state"/>
    </xsl:when>
  </xsl:choose>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="error">
  <xsl:param name="pad"/>
  <xsl:text>    </xsl:text>
  <xsl:call-template name="rpad">
    <xsl:with-param name="str" select="string(@symbol)"/>
    <xsl:with-param name="pad" select="number($pad) + 2"/>
  </xsl:call-template>
  <xsl:text>error</xsl:text>
  <xsl:text> (</xsl:text>
  <xsl:value-of select="text()"/>
  <xsl:text>)</xsl:text>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="reduction">
  <xsl:param name="pad"/>
  <xsl:text>    </xsl:text>
  <xsl:call-template name="rpad">
    <xsl:with-param name="str" select="string(@symbol)"/>
    <xsl:with-param name="pad" select="number($pad) + 2"/>
  </xsl:call-template>
  <xsl:if test="@enabled = 'false'">
    <xsl:text>[</xsl:text>
  </xsl:if>
  <xsl:choose>
    <xsl:when test="@rule = 'accept'">
      <xsl:text>accept</xsl:text>
    </xsl:when>
    <xsl:otherwise>
      <xsl:text>reduce using rule </xsl:text>
      <xsl:value-of select="@rule"/>
      <xsl:text> (</xsl:text>
      <xsl:value-of
	  select="/bison-xml-report/grammar/rules/rule[@number = current()/@rule]/lhs[text()]"/>
      <xsl:text>)</xsl:text>
    </xsl:otherwise>
  </xsl:choose>
  <xsl:if test="@enabled = 'false'">
    <xsl:text>]</xsl:text>
  </xsl:if>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="solved-conflicts">
  <xsl:if test="resolution">
    <xsl:text>&#10;</xsl:text>
    <xsl:apply-templates select="resolution"/>
  </xsl:if>
</xsl:template>

<xsl:template match="resolution">
  <xsl:text>    Conflict between rule </xsl:text>
  <xsl:value-of select="@rule"/>
  <xsl:text> and token </xsl:text>
  <xsl:value-of select="@symbol"/>
  <xsl:text> resolved as </xsl:text>
  <xsl:if test="@type = 'error'">
    <xsl:text>an </xsl:text>
  </xsl:if>
  <xsl:value-of select="@type"/>
  <xsl:text> (</xsl:text>
  <xsl:value-of select="."/>
  <xsl:text>).&#10;</xsl:text>
</xsl:template>

<xsl:template name="max-width-symbol">
  <xsl:param name="node"/>
  <xsl:variable name="longest">
    <xsl:for-each select="$node">
      <xsl:sort data-type="number" select="string-length(@symbol)"
		order="descending"/>
      <xsl:if test="position() = 1">
	<xsl:value-of select="string-length(@symbol)"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:variable>
  <xsl:value-of select="$longest"/>
</xsl:template>

<xsl:template name="lpad">
  <xsl:param name="str" select="''"/>
  <xsl:param name="pad" select="0"/>
  <xsl:variable name="diff" select="$pad - string-length($str)" />
  <xsl:choose>
    <xsl:when test="$diff &lt; 0">
      <xsl:value-of select="$str"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="space">
	<xsl:with-param name="repeat" select="$diff"/>
      </xsl:call-template>
      <xsl:value-of select="$str"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="rpad">
  <xsl:param name="str" select="''"/>
  <xsl:param name="pad" select="0"/>
  <xsl:variable name="diff" select="$pad - string-length($str)"/>
  <xsl:choose>
    <xsl:when test="$diff &lt; 0">
      <xsl:value-of select="$str"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$str"/>
      <xsl:call-template name="space">
	<xsl:with-param name="repeat" select="$diff"/>
      </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="space">
  <xsl:param name="repeat">0</xsl:param>
  <xsl:param name="fill" select="' '"/>
  <xsl:if test="number($repeat) &gt;= 1">
    <xsl:call-template name="space">
      <xsl:with-param name="repeat" select="$repeat - 1"/>
      <xsl:with-param name="fill" select="$fill"/>
    </xsl:call-template>
    <xsl:value-of select="$fill"/>
  </xsl:if>
</xsl:template>

<xsl:template name="line-wrap">
  <xsl:param name="line-length" required="yes" />
  <xsl:param name="first-line-length" select="$line-length" />
  <xsl:param name="text" required="yes" />
  <xsl:choose>
    <xsl:when test="string-length($text) = 0 or normalize-space($text) = ''" />
    <xsl:when test="string-length($text) &lt;= $first-line-length">
      <xsl:value-of select="concat($text, '&#10;')" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:variable name="break-pos">
        <xsl:call-template name="ws-search">
          <xsl:with-param name="text" select="$text" />
          <xsl:with-param name="pos" select="$first-line-length+1" />
        </xsl:call-template>
      </xsl:variable>
      <xsl:value-of select="substring($text, 1, $break-pos - 1)" />
      <xsl:text>&#10;</xsl:text>
      <xsl:call-template name="line-wrap">
        <xsl:with-param name="line-length" select="$line-length" />
        <xsl:with-param
          name="text" select="concat('    ', substring($text, $break-pos+1))"
        />
      </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="ws-search">
  <xsl:param name="text" required="yes" />
  <xsl:param name="pos" required="yes" />
  <xsl:choose>
    <xsl:when
      test="$pos &gt; string-length($text) or substring($text, $pos, 1) = ' '"
    >
      <xsl:value-of select="$pos" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="ws-search">
        <xsl:with-param name="text" select="$text" />
        <xsl:with-param name="pos" select="$pos+1" />
      </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

</xsl:stylesheet>
