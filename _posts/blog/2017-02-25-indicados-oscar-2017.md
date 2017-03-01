---
category: "blog"
title: "Indicados ao Oscar 2017"
tags: ["oscar", "premios"]

---
Textos escritos para os indicados ao Oscar desse ano, por ordem de publicação.

{% for post in site.posts %}
{% if post.tags contains "oscar2017" %}
 - [{% include all_item_ranking.html %} {{ post.title }}]({{ post.url }})
{% endif %}
{% endfor %}
