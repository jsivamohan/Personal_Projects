<!DOCTYPE html>
<html lang = "en">
<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<body>
<?php
//this page display a greeting to the user, a login button, stories and who they were written by, with links to those stories
//likes for each story, and a search bar.
//in addition, if the user is login in, there is a logout button, 
//a add story button and options to edit or delete any stories they have written
require 'database.php';
session_start();


if($_SESSION["user_id"]==NULL){
    $_SESSION["user_id"] = -1;
}
if(empty($_SESSION["user_id"]) || $_SESSION["user_id"]<0){
    ?>
    <form action=Login_page.php method="post">
        <p>
            <input type=submit value=Login />
        </p>
    </form>
    <?php
    echo "Hi User";
    echo "<br>";

}
else{
    $username = $_SESSION["user_name"];
    ?>
    <form action=News_page_logout.php method="post">
        <p>
            <input type=submit value=Logout />
        </p>
    </form>
    <form action= add_story.php method="post">
        <p>
            <input type=submit value="Add Story" />
        </p>
    </form>
    <?php
    echo "Hi: $username";
    echo "<br>";
}
?>
<form action=display_users.php method="post">
    <p>
        <input type=submit value="Display All Users" />
    </p>
</form>

<form action="News_page.php" method="post">
    <p>
        <label for="search">Search through stories (search for nothing to view all stories)</label>
        <input type="text" name="search" id="search" />
    </p>

    <input type="submit" value="Search" />
    <input type="reset" />
</form>

<?php
//php file for the general news site display page (has all of the stories along with a little blurb that's just the start of
//the story)

$story_write_arr = array();
$story_ID_arr = array();
$user_Id_arr = array();
$stories_arr = array();
$body_arr = array();
$link_arr = array();
$likes_arr = array();

$searchFor =  $_POST["search"];


//sql orders by likes when displaying 
$stmt = $mysqli->prepare("select stories.storiesID, users.username, stories.userID, stories.title, stories.body, stories.likes from stories join users on (stories.userID = users.userID) where stories.title like '%$searchFor%' order by likes desc");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->execute();

$result = $stmt->get_result();


while($row = $result->fetch_assoc()){
    array_push($story_write_arr, $row["username"]);
    array_push($story_ID_arr, $row["storiesID"]);
    array_push($user_Id_arr, $row["userID"]);
    array_push($stories_arr, $row["title"]);
    array_push($body_arr, $row["body"]);
    array_push($likes_arr, $row["likes"]);
}

$stmt = $mysqli->prepare("select stories.storiesID, users.username, stories.userID, stories.title, stories.body, stories.likes from stories join users on (stories.userID = users.userID) where stories.body like '%$searchFor%' and stories.title not like '%$searchFor%' order by likes desc");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->execute();

$result = $stmt->get_result();


while($row = $result->fetch_assoc()){
    array_push($story_write_arr, $row["username"]);
    array_push($story_ID_arr, $row["storiesID"]);
    array_push($user_Id_arr, $row["userID"]);
    array_push($stories_arr, $row["title"]);
    array_push($body_arr, $row["body"]);   array_push($likes_arr, $row["likes"]);
    array_push($likes_arr, $row["likes"]);

}

$stmt->close();

?>

<h2>Stories</h2>
<?php


for($i=0; $i<count($stories_arr); $i++){
    $story = $stories_arr[$i];
    $story_ID = $story_ID_arr[$i];
    $user = $user_Id_arr[$i];
    $link = $link_arr[$i];
    $story_write = $story_write_arr[$i];
    $likes = $likes_arr[$i];
    //stores the number of likes for the given story in the variable likes
    
    //gets the user_id for this story and save into variable user, as well as the story_id for this story
    //and store into variable story_ID
    ?>
    <h3>
    <a href= "story_viewing_set_sessions.php?story_id=<?php echo $story_ID;?>"> <?php echo $story;?></a>
    </h3>

    written by <?php echo $story_write;?>
    <br>
    Likes: <?php echo $likes;?>   
    
    <form action = "like_story.php" method = "post">
        <input type = "hidden" name = "story_id" value = "<?php echo $story_ID;?>" > 
        <input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
        <input type= "submit" value= "like" />
    </form>
    <br>
    <br>
    <?php
}
?> 
</body>
</html>