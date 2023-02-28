
<!DOCTYPE html>
<html lang = "en">
<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<?php
require 'database.php';
//Sql selects all usernames from the users database in array $users (groupby user_id for all cases) and store the counts for the 
//number of stories they've made along with the
//numbers of stories they've made in $stories array, and the number of comments they've made in the $comments array
//in addition, deleted users are not shown
$users = array();
$stories_count = array();
$comments_count = array();

$stmt = $mysqli->prepare("SELECT users.username, count(comments.commentID) as commentCount FROM `users` left join comments on (users.userID = comments.userID)  where users.username not like 'user unknown' GROUP BY users.userID");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->execute();
$result = $stmt->get_result();

while($row = $result->fetch_assoc()){
    
    array_push($users, $row["username"]);
    array_push($comments_count, $row["commentCount"]);
}
$stmt->close();
$stmt = $mysqli->prepare("SELECT count(stories.storiesID) as storyCount FROM `users` left join stories on (users.userID = stories.userID)  where users.username not like 'user unknown' GROUP BY users.userID");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->execute();
$result = $stmt->get_result();

while($row = $result->fetch_assoc()){
    array_push($stories_count, $row["storyCount"]);
}
$stmt->close();


echo "<h2>Users</h2>";
echo "<div class  = box>";
for($i=0; $i<count($stories_count); $i++){
    echo "<h3>";
    echo "user: $users[$i]"; 
    echo "<br>";
    echo "stories: $stories_count[$i] comments: $comments_count[$i]";
    echo "</h3>";
    echo "<br>";

}
echo "</div>";
?>
<form action="News_page.php">
	<p>
		<input type="submit" value="Back to News Page" />
	</p>
</form>
</body>
</html>