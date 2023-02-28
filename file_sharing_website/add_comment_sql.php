
<?php
//this page adds a comment to the database if the session token is the posted token
require 'database.php';
session_start();
$comment = $mysqli->real_escape_string($_POST['Comment']);
echo $_SESSION['token'] == $_POST['token'];
if(!hash_equals($_SESSION['token'], $_POST['token'])){
	die("Request forgery detected");
}

$stmt = $mysqli->prepare("insert into comments (storyID, userID, text) values (?,?,?)");
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        exit;
    }

    $stmt->bind_param('iis', $_SESSION['story_id'], $_SESSION['user_id'] , $comment);
    $stmt->execute();
    $stmt->close();

//****add the comment (using the story_id and user_id sessions) into the comments database */


header("Location: story_viewing.php");
exit; 

?>