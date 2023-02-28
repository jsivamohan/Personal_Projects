<!DOCTYPE html>
<html lang = "en">
<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<body>
<?php
//file to edit the value of a story
//this page first takes in the value that the user would like to change
//and then takes in what they would like to change the area to
session_start();
if(empty($_POST['operation'])){
    ?>
    <form action="edit_story.php" method="post">
    <p>
		<strong>What do you want to change:</strong>
		<input type="radio" name="operation" value="Title" id="Title" /> <label for="Title">Title</label> &nbsp;
		<input type="radio" name="operation" value="Body" id="Body" /> <label for="Body">Body</label>
        <input type="radio" name="operation" value="link" id="link" /> <label for="link">Link</label>
	</p>
	<p>
		<input type="submit" value="Send" />
		<input type="reset" />
	</p>
    </form>
    <?php
}
else{
    $_SESSION['operation'] = $_POST['operation'];
    $operation = $_SESSION['operation'];
    $operation = $_POST['operation'];
    echo $_POST['operation'];
    ?>
    <form action="edit_story_sql.php" method= "post">
        <h2>
            <?php
            "<label for=change>$operation:</label>"?>
            <input type=text name=change id=change />
            <input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
        </h2>

        <p>
            <input type=submit value=Change $operation/>
            <input type=reset />
            <input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
        </p>
    </form>
    <?php
}
echo "<form action=story_viewing.php>";
	echo "<p>";
		echo "<input type=submit value= 'Back to story' />";
	echo "</p>";
echo "</form>";
?>
</body>
</html>

