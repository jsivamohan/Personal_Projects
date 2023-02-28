<?php
//this page deletes a comment from the comments table, if tokens match
require 'database.php';

session_start();
$comment_id = $mysqli->real_escape_string($_POST["comment_id"]);
echo $comment_id;
echo $_SESSION['token'] == $_POST['token'];
if(!hash_equals($_SESSION['token'], $_POST['token'])){
	die("Request forgery detected");
}
$stmt = $mysqli->prepare("delete from comments where commentID = ?");
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        exit;
    }
    $stmt->bind_param('i', $comment_id);
    $stmt->execute();
    $stmt->close();

    header("Location: story_viewing.php");
    exit;
?>