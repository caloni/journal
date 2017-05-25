---
layout: default
title: "Escovando bits desde 1997."
main: true
---
  <div class="search">
    <form id="cse-search-box" action="">
      <span class="fa fa-search"></span>
      <input id="from-query" type="text" name="q" style="width: 100%; font-size: 22px; margin-bottom: 6px;" />
    </form>
  <div id="cse" style="width: 100%;">Loading</div>
  </div>
<script type="text/javascript" src="http://www.google.com/jsapi"></script>
<script type="text/javascript">// <![CDATA[
function parseQueryFromUrl () {
        var queryParamName = "q";
        var search = window.location.search.substr(1);
        var parts = search.split('&');
        for (var i = 0; i < parts.length; i++) {
          var keyvaluepair = parts[i].split('=');
          if (decodeURIComponent(keyvaluepair[0]) == queryParamName) {
            return decodeURIComponent(keyvaluepair[1].replace(/\+/g, ' '));
          }
        }
        return '';
      }
      google.load('search', '1', {language : 'pt'});
      google.setOnLoadCallback(function() {
        var customSearchControl = new google.search.CustomSearchControl('{{ site.google_search_customsearchcontrol }}');
        customSearchControl.setResultSetSize(google.search.Search.FILTERED_CSE_RESULTSET);
        var options = new google.search.DrawOptions();
        options.enableSearchResultsOnly();
        customSearchControl.draw('cse', options);
        var queryFromUrl = parseQueryFromUrl();
        if (queryFromUrl) {
          customSearchControl.execute(queryFromUrl);
        }
      }, true);
// ]]></script>

{% for post in site.categories.pinned %}
{% include index_item_content.html %}
{% endfor %}

{% for post in site.posts limit:20  %}
{% include index_item_content.html %}
{% endfor %}
<div style="text-align: right; font-size: small; margin-bottom: 25px;">
    <a href="/all" title="Ver todos os posts (isso pode ser looooooooooongo)"><i>{{ site.name }} - {{ site.time | date: "%Y-%m-%d %H:%M:%S" }}</i></a>
</div>
