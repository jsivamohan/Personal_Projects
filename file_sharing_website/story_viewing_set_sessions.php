<?php
//sets the session story id to the current story
session_start();
$_SESSION['story_id'] = $_GET['story_id'];

echo $_SESSION['story_id'];

header('Location: story_viewing.php');
exit;
?>