<?php

$bitData = json_decode($_POST["payload"]);

foreach( $bitData->commits as $c )
{
    $msg = $c->message;
    $pattern = '#http[s]*://trello.com/c/([A-Za-z0-9]+)#';
    if( preg_match($pattern, $msg, $matches) == 0 )
        continue;

    $url = 'https://trello.com/1/cards/' . $matches[1] . '/actions/comments';
    $msg = $c->message . ' Commit: ' . $bitData->canon_url . $bitData->repository->absolute_url . 'commits/' . $c->raw_node;
    $data = array(
            'key' => 'AQUI_VAI_SUA_CHAVE', 
            'token' => 'AQUI_VAI_SEU_TOKEN_DE_ACESSO',
            'text' => $msg
            );

    $options = array(
            'http' => array(
                'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
                'method'  => 'POST',
                'content' => http_build_query($data),
                ),
            );

    $context  = stream_context_create($options);

    $result = file_get_contents($url, false, $context);
}
?>

