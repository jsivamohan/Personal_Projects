<?php
require 'database.php';
//this page deletes a user if they entered a correct username and matching password
session_start();
$username = $mysqli->real_escape_string($_POST['Username']);
$password = $mysqli->real_escape_string($_POST['Password']);
if(!hash_equals($_SESSION['token'], $_POST['token'])){
	die("Request forgery detected");
}

if (empty($_SESSION['Password'])){
    $_SESSION["user_id"] = -1;
    $_SESSION["user_name"] = "user";
    header("Location: Login_page.php");
    exit; 
}

$stmt = $mysqli->prepare("select userID, username, password from users where username like ?");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->bind_param('s', $username);
$stmt->execute();
$stmt->bind_result($id, $username, $password);
$stmt->fetch();
$stmt->close();

$continue = FALSE;
//makes sure that usernames are the same
if(password_verify($_SESSION['Password'],$password) && $username == $_SESSION['Username']){
    echo('passwords match');
    $continue = TRUE;
    
}else{
    $_SESSION["user_id"] = -1;
    $_SESSION["user_name"] = "user";
}

if($continue){
    $stmt = $mysqli->prepare("update users set username = 'User Unknown' where userID = ?");
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        exit;
    }

    $stmt->bind_param('s', $id);
    $stmt->execute();
    $stmt->fetch();
    $stmt->close();
}
echo $id;
header("Location: Login_page.php");
exit; 
//sql here to checks if the username and password match a user in the users database, if so update that user to be 
//username deleted user and send back to the login page, if not then send back to the delete user page


?>