<?php
require 'database.php';
//sql to updates the story like counter by one for the story with the given story_ID and then go back to the News_page
session_start();

$story_ID = $_POST['story_id'];
echo "$story_ID";
$current_likes;

$stmt = $mysqli->prepare("select likes from stories where storiesID = ?");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->bind_param('i', $story_ID);
$stmt->execute();
$stmt->bind_result($current_likes);
$stmt->fetch();
$stmt->close();
echo $current_likes;

$stmt = $mysqli->prepare("update stories set likes = ? where storiesID = ?");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}
$current_likes = $current_likes + 1;
$stmt->bind_param('ii', $current_likes, $story_ID);
$stmt->execute();
$stmt->fetch();
$stmt->close();

header('Location: News_page.php');
exit;
?>