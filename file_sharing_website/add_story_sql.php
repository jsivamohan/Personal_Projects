<?php
//this page adds a story to the database with the title, body, and link that is provided, if tokens match
session_start();
require 'database.php';

echo $_SESSION['token'];
echo "<br>";
echo $_POST['token'];

if(!hash_equals($_SESSION['token'], $_POST['token'])){
	die("Request forgery detected");
}

$user = $mysqli->real_escape_string($_SESSION['user_id']);
$title = $mysqli->real_escape_string($_POST['Title']);
$body = $mysqli->real_escape_string($_POST['Body']);
$link = $mysqli->real_escape_string($_POST['Link']);


$stmt = $mysqli->prepare("insert into stories (userID, title, body, link) values (?,?,?,?)");
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        exit;
    }

    $stmt->bind_param('isss', $user, $title, $body, $link);
    $stmt->execute();
    $stmt->close();

header("Location: News_page.php");
exit;

?>