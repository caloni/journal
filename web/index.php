<html>
<head>
<meta charset="ISO-8859-1">
<script src="sorttable.js"></script>
<style>
/* Sortable tables */
table.sortable thead {
    background-color:#eee;
    color:#217e22;
    text-decoration: underline;
    font-weight: bold;
    cursor: default;
}
tr.header { cursor: hand; }
      a {
        color: #217e22;
        text-decoration: none;
      }
tr
{
font-family: '{text:GoogleWebFont}', Georgia, serif;
}
</style>
</head>
<body>
<?php
$spreadsheet_url="https://raw.githubusercontent.com/Caloni/CineTenisVerde/master/lists/index.csv";
$jsTarget = ' onclick="return ! window.open(this.href);"';

function usestar($rating)
{
	$output = str_repeat ('&#9733;', $rating) . str_repeat ('&#9734;', 5 - $rating);
	return $output;
}

if(!ini_set('default_socket_timeout', 15)) echo "<!-- unable to change socket timeout -->";

if(($handle = fopen($spreadsheet_url, "r")) !== FALSE)
{
    echo '<table border="0" class="sortable">';
    if(($data = fgetcsv($handle, 1000, ",")) !== FALSE)
    {
        echo '<tr class="header">';
        $tenis = $data[0];
        $imdb = $data[1];
        $title = $data[2];
        $rating = $data[6];
        //echo '<th>' . '#' . '</th>';
		echo '<th>' . 'Data' . '</th>';
        echo '<th>' . $title . '</th>';
        //echo '<th>' . $tenis . '</th>';
        echo '<th>' . $rating . '</th>';
        echo '<th>' . $imdb . '</th>';
        echo '</tr>';

        while(($data = fgetcsv($handle, 1000, ",")) !== FALSE)
        {
            echo '<tr>';
            //$number = $data[0];
            $tenis = $data[0];
            $tenisLink = 'https://github.com/Caloni/CineTenisVerde/blob/master/reviews/' . $data[0] . '.md' ;
            $imdb = $data[1];
            $imdbLink = 'http://www.imdb.com/title/' . $data[1];
            $title = $data[2];
			$dt = $data[3];
            $rating = $data[6];
            $star = usestar(intval($rating));
            //echo '<th>' . $number . '</th>';
			echo '<th>' . $dt . '</th>';
            //echo '<th>' . $title . '</th>';
			echo '<th><a href="' . $tenisLink . '"' . $jsTarget . '>' . $title . '</a></th>';
            //echo '<th><a href="' . $tenisLink . '"' . $jsTarget . '>' . $tenis . '</a></th>';
            echo '<th><b><font color="#fcd200" size="5">' . $star . '</font></b></th>';
            echo '<th><a href="' . $imdbLink . '"' . $jsTarget . '>' . $imdb. '</a></th>';
            echo '</tr>';
        }
    }
    echo '</table>';
    fclose($handle);
}
else die("Problem reading csv");

?>
</body>
</html>