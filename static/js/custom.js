$(function() {
    $('img').each(function () {
        $(this).wrap('<a href="' + $(this).attr('src') + '"></a>');
    });
});
