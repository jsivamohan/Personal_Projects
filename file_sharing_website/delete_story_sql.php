<?php
session_start();
//this page deletes a story and all comments that have that story ID
require 'database.php';
$stmt = $mysqli->prepare("delete from comments where storyID = ?");
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        exit;
    }
    $stmt->bind_param('i', $_SESSION['story_id']);
    $stmt->execute();
    $stmt->close();

 $stmt = $mysqli->prepare("delete from stories where storiesID = ?");
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        exit;
    }
    $stmt->bind_param('i', $_SESSION['story_id']);
    $stmt->execute();
    $stmt->close();

    header("Location: News_page.php");
    exit;
?>