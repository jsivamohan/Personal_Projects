<!DOCTYPE html>
<html lang = "en">
<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<body>

<form action="News_page.php">
	<p>
		<input type="submit" value="Back to News Page" />

	</p>
</form>
<?php
require 'database.php';




//php file for viewing a single story, has the story header along with it's comments, queries the story database based on the 
//story id passed from the news page to display the story title, body from the story table, along with the comments from the 
//foreign key in the comments table pointing to the id of the story it's related to. Checks to see whether or not the user that 
//selected the story is the one who wrote it and if it is then allows them to edit the page.

//also queries the comments to see if the user who wrote the comment is the one who clicked on the page. If so will allow them to 
//edit the comment or delete it.

session_start();
$_SESSION['comment_id']= NULL;
echo "<h3>Hello: $_SESSION[user_name]</h3>";

$stmt = $mysqli->prepare("SELECT stories.title, stories.body, stories.userID, stories.link, users.username from stories JOIN users on (stories.userID = users.userID) where stories.storiesID = ?");

if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->bind_param('s', $_SESSION['story_id']);
$stmt->execute();
$stmt->bind_result($title, $body, $story_creator, $link, $creator_name);
$stmt->fetch();
$stmt->close();


$comments = array();
$comment_ids = array();
$comment_key_ID = array();
$comment_usernames_arr = array();

$stmt = $mysqli->prepare("SELECT comments.commentID, comments.text, comments.userID, users.username FROM stories JOIN comments on (stories.storiesID = comments.storyID) join users on (comments.userID = users.userID) where storyID = ?");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->bind_param('s', $_SESSION['story_id']);
$stmt->execute();
$result = $stmt->get_result();

while($row = $result->fetch_assoc()){

    array_push($comment_key_ID, $row["commentID"]);
    array_push($comments, $row["text"]);
    array_push($comment_ids, $row["userID"]);
    array_push($comment_usernames_arr, $row["username"]);
}
$stmt->close();


//need to get the story title from the story id and save into variable $title, get the body of the story and save into 
//$body and save all the comments into array $comments
//save the id of the story itself into variable story_creator
if(empty($title)){
    $title = "filler title";
}
if(empty($body)){
    $body = "filler body";
}

echo "<h1>$title</h1>";

echo "<h3> created by: $creator_name </h3>";


echo "<h2>$body</h2>";
echo "<br>";
if($link != NULL){
    echo "<a href=$link>Link to relevant info</a>";
    echo "<br>";
}
if($story_creator == $_SESSION["user_id"]){
    ?>
    <form action = edit_story.php method = "post">
    <p>
        <input type=submit value="edit story" />
    </p>
    </form> 
    <form action = delete_story.php>
    <p>
        <input type=submit value= "delete story" />
    </p>
    </form>
    <?php
}

if($_SESSION["user_id"] > 0){
echo "<form action=add_comment.php>";
	echo "<p>";
		echo "<input type=submit value= 'Add comment' />";
	echo "</p>";
echo "</form>";
}
?>
<?php

echo "comments:";
echo "<br>";
echo "<br>";
$comment_name = NULL;
$comment_id = NULL;
for($i=0; $i<count($comments); $i++){
    //use sql here to get the user_id for this story and save into variable user, as well as the story_id for this story
    //and store into variable story_ID
    echo "$comments[$i]";
    $comment_id = $comment_key_ID[$i];
    //through each iteration of the for loop, set the id variable to be the user_id for that comment 
    echo "<br>";
    echo "commented by: $comment_usernames_arr[$i]";
    if($comment_ids[$i]==$_SESSION['user_id']){
    ?>
    <form action = edit_comment.php method= post>
        <p>
            <input type="submit" value="edit comment" />
            <input type= "hidden" name= "comment_id" value= "<?php echo $comment_key_ID[$i];?>"/> 
        </p>
    </form>

    <form action = delete_comment.php method= post>
        <p>
            <input type="submit" value="delete comment" />
            <input type= "hidden" name= "comment_id" value= "<?php echo $comment_key_ID[$i];?>"/> 
            <input type= "hidden" name= "token" value= "<?php echo $_SESSION['token'];?>"/> 
        </p>
    </form>
    <?php
    }


    // echo "<br>";
    // echo "<a href=comment_viewing.php?comment_id=$comment_id>View Comment</a>";
    echo "<br>";
    echo "<br>";
}

?>





</body>
</html>