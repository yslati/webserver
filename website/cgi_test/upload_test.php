<?php


print_r($_FILES);
// Check if image file is a actual image or fake image
if (isset($_POST)) {
	echo "OK";
	move_uploaded_file($_FILES["t"]["tmp_name"], __DIR__ . "/ok3.go");
}
