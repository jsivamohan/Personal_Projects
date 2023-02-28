<?php
//this page edits a comment to say something new if tokens match
require 'database.php';
echo "got to add_comment";
session_start();
$comment_id = $_SESSION['comment_id'];
$comment_val = $mysqli->real_escape_string($_POST['Comment']);
echo $comment_val;
echo $comment_id;

if(!hash_equals($_SESSION['token'], $_POST['token'])){
	die("Request forgery detected");
}

$stmt = $mysqli->prepare("update comments set text= ? where commentID = ?");
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        exit;
    }

    $stmt->bind_param('si', $comment_val, $comment_id);
    $stmt->execute();
    $stmt->close();

//update the comment (using the comment_id var with comment_val) in the comments database and go back to News_page*/

header("Location: story_viewing.php");
exit;

?>