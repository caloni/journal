---
layout: default
title: "A melhor forma de ver um filme é com o cérebro."
main: true
---
{% for post in site.posts %}
{% assign page = post %}
{% assign content = post.excerpt %}
  <div id="post-info">
    <div id="cover-photo-container">
      <img id="cover-photo" src="/images/screenshots{{ page.id }}.jpg">
    </div>
    <div id="info-container">
      <div id="title"><a href="{{ post.url }}">{{ page.title }}</a></div>
      <span id="details">{% if page._director %}{{ page._director | join: ', ' }}, {% endif %}{% if page._year %}{{ page._year }}{% endif %}</span>
    </div>
    <div class="post">
      {{ content }}
    </div>
  </div>
{% endfor %}
<div style="text-align: right; font-size: small; margin-bottom: 25px;">
    <a href="/" title="Voltar pra Home"><i>Cine Tênis Verde - {{ site.time | date: "%Y-%m-%d %H:%M:%S" }}</i></a>
</div>
