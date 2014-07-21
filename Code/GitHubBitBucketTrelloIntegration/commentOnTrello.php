<?php

$url = 'https://trello.com/1/cards/ID_DO_CARD/actions/comments';

$msg = 'Hello, World!';

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

?>

