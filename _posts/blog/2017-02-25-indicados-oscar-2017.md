---
category: "blog"
stars: "5/5"
title: "Indicados ao Oscar 2017"
tags: ["oscar", "premios"]

---
Textos escritos para os indicados ao Oscar desse ano, por ordem de publicação.

<table>
{% for post in site.posts %}
{% if post.tags contains "oscar2017" %}
<tr><td>{{ post.date | date: '%Y-%m-%d' }}</td><td>{% include all_item_ranking.html %}</td><td><a href="{{ post.url }}">{{ post.title }}</a></td></tr>
{% endif %}
{% endfor %}
</table>
