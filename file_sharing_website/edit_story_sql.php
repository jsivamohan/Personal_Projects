<?php
//this page edits the contents of a story based on the section that the user wants to edit, if the tokens match
require 'database.php';
session_start();
$operation = $_SESSION['operation'];
$change = $mysqli->real_escape_string($_POST['change']);
echo $_SESSION['token'] == $_POST['token'];
if(!hash_equals($_SESSION['token'], $_POST['token'])){
	die("Request forgery detected");
}

if($operation == "Title"){
    $stmt = $mysqli->prepare("update stories set title = ? where storiesID = ?");
}
else if($operation == "Body"){
    $stmt = $mysqli->prepare("update stories set body = ? where storiesID = ?");
}
else{
    $stmt = $mysqli->prepare("update stories set link = ? where storiesID = ?");
}
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        exit;
    }
echo "done";

    $stmt->bind_param('si', $change, $_SESSION['story_id']);
    $stmt->execute();
    $stmt->close();
    header("Location: story_viewing.php");
    exit;

?>