---
layout: default
title: "Pesquise na lista de todos os posts"
---
<script src="{{ site.baseurl }}/scripts/jquery-2.1.3.min.js"></script>
<script>
function ApplyFilter(filter)
{
        query = filter
        query = $.trim(query); //trim white space
        query = query.split('&');

        if( query.length == 1 )
        {
            queryRegex = $.trim(query[0]);
            regex = new RegExp(queryRegex, "i");
            hides = 0;
            shows = 0;

            $.each( $('.sortable'), function(i, sortable) {
              $('tr', sortable).each(function() {
                var srch = $(this).text() + $(this).find('a').prop('title');
                if( srch.search(regex) < 0 )
                {
                    $(this).hide()
                    hides++;
                }
                else
                {
                    $(this).show();
                    shows++;
                }
                });
            });

            total = shows + hides;
            $('#filter').prop('title', shows + ' de ' + total);
        }
        else 
        {
            total = 0;
            // mostra todo mundo
            $.each( $('.sortable'), function(i, sortable) {
              $('tr', sortable).each(function() {
                $(this).show()
                total++;
                });
            });
            shows = total;

            // agora vai escondendo
            for( var i = 0; i < query.length; ++i )
            {
                queryRegex = $.trim(query[i]);
                regex = new RegExp(queryRegex, "i");

                $.each( $('.sortable'), function(i, sortable) {
                  $('tr', sortable).each(function() {
                    var srch = $(this).text() + $(this).find('a').prop('title');
                    if( srch.search(regex) < 0 )
                    {
                        if( $(this).is(":visible") )
                        {
                            shows--;
                            $(this).hide()
                        }
                    }
                    });
                });
            }

            $('#filter').prop('title', shows + ' de ' + total);
        }
}

var QueryString = function () {
  // This function is anonymous, is executed immediately and 
  // the return value is assigned to QueryString!
  var query_string = {};
  var query = window.location.search.substring(1);
  var vars = query.split("&");
  for (var i=0;i<vars.length;i++) {
    var pair = vars[i].split("=");
        // If first entry with this name
    if (typeof query_string[pair[0]] === "undefined") {
      query_string[pair[0]] = decodeURIComponent(pair[1]);
        // If second entry with this name
    } else if (typeof query_string[pair[0]] === "string") {
      var arr = [ query_string[pair[0]],decodeURIComponent(pair[1]) ];
      query_string[pair[0]] = arr;
        // If third or later entry with this name
    } else {
      query_string[pair[0]].push(decodeURIComponent(pair[1]));
    }
  } 
  return query_string;
}();

$(document).ready(function(){
    $("#filter").keyup(function(event){
        query = $(this).val();
        ApplyFilter(query);
    });
    var query = QueryString['q'];
    $('#filter').val(query);
    $('#filter').focus();
    ApplyFilter(query);
});

</script>

  <table class="sortable">
  <input type="text" name="filter" value="" id="filter" style="width: 100%; font-size: 22px;" title=""/>
  {% for post in site.posts %}
  <tr><td>
        <a href="{{ post.url }}" 
        title="{% include all_item_tooltip.html %}"
>
		{% include all_item_ranking.html %}

        {{ post.title }}

		{% include all_item_details.html %}

        </a>
  </td></tr>
  {% endfor %}
  </table>
<div style="text-align: right; font-size: small; margin-bottom: 25px;">
    <a href="/" title="Voltar para Home"><i>{{ site.name }} - {{ site.time | date: "%Y-%m-%d %H:%M:%S" }}</i></a>
</div>
