---
category: "blog"
title: "Indicados ao Oscar 2017"
tags: ["oscar", "premios"]

---
  {% for post in site.posts %}
  {% if post.tags contains "oscar2017" %}
  - [{% include all_item_ranking.html %} {{ post.title }}]({{ post.url }})
  {% endif %}
  {% endfor %}
