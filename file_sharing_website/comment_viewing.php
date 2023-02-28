<?php
//set the sessions of the php for comment viewing
require 'database.php';

session_start();
$_SESSION['comment_id'] = $mysqli->real_escape_string($_POST['comment_id']);

$user = NULL;
$comment_body = NULL;
$user_id = NULL;

$stmt = $mysqli->prepare("SELECT users.username, comments.text, comments.userID from comments join users on (users.userID = comments.userID) where commentID = ?");

if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->bind_param('i', $_SESSION['comment_id']);
$stmt->execute();
$stmt->bind_result($writer, $comment_body, $user_id);
$stmt->fetch();
$stmt->close();

echo $comment_body;
echo "<br>" ;
echo "Written by: $writer";
//gets the text related to the comment based on the comment ID (store in comment_body)
//and show here, along with who made it (stored in user), store the ID of who made it in user_id
//from the same table

if($user_id == $_SESSION['user_id']){
    echo "<form action = edit_comment.php>";
        echo "<p>";
            echo "<input type=submit value=edit comment />";
        echo "</p>";
    echo "</form>";

    echo "<form action = delete_comment.php>";
        echo "<p>";
            echo "<input type=submit value=delete comment />";
        echo "</p>";
    echo "</form>";
}
?>
<form action="story_viewing.php">
	<p>
		<input type="submit" value="Back to Story" />
	</p>
</form>