<?php
require 'database.php';
// this page checks that the username matches a username in the database,
// and that the corresponding hashed password matches the provided password
session_start();

$_SESSION['Username'] = $mysqli->real_escape_string($_POST['Username']);
$_SESSION['Password'] = $mysqli->real_escape_string($_POST['Password']);
$_SESSION['token'] = bin2hex(openssl_random_pseudo_bytes(32));
if (empty($_SESSION['Password'])){
    $_SESSION["user_id"] = -1;
    $_SESSION["user_name"] = "user";
    header("Location: Login_page.php");
    exit; 
}

$stmt = $mysqli->prepare("select userID, username, password from users where username like ? and username not like 'user unknown'");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->bind_param('s', $_SESSION['Username']);
$stmt->execute();
$stmt->bind_result($id, $username, $password);
$stmt->fetch();
$stmt->close();

if(password_verify($_SESSION['Password'],$password) && $username == $_SESSION['Username']){
    echo('passwords match');
    $_SESSION["user_id"] = $id;
    $_SESSION["user_name"] = $username;
    $_SESSION['token'] = bin2hex(openssl_random_pseudo_bytes(32));
    header("Location: News_page.php");
    exit; 
}else{
    $_SESSION["user_id"] = -1;
    $_SESSION["user_name"] = "user";
}
header("Location: Login_page.php");
exit; 


?>