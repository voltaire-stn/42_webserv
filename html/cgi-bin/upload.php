#!/usr/bin/php

<?php
$uploaddir = $_SERVER['DOCUMENT_ROOT'] . getenv('UPLOAD_DIR');
$uploadfile = $uploaddir . basename($_FILES['file']['name']);

if (!is_dir($uploaddir))
    mkdir($uploaddir, 0775);

echo '<pre>';
if (move_uploaded_file($_FILES['file']['tmp_name'], $uploadfile)) {
    echo "<h1>File successfully uploaded !</h1>\n";
} else {
    echo "Downloading error, please see below:\n";
    print("uploaddir   : $uploaddir</br>\n");
    print_r($_FILES);
}
print("uploadfile  : $uploadfile</br>\n</br>\n");

echo '</pre>';
?>
