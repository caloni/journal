---
layout: nil
---
$(function() {

var availableTitles = [
{% for post in site.posts %}
  { value: "{{ post.url }}",
    label: "{% for tag in post.tags %}{{ tag }} {% endfor %}"
  },
  { value: "{{ post.url }}",
    label: "{{ post.title }}"
  },
{% endfor %}
];

$( "#search-autocomplete" ).autocomplete({
source: availableTitles,
select: function(event, ui) {
  window.location.href = ui.item.value;
  }
});

});

