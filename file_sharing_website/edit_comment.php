<!DOCTYPE html>
<html lang = "en">

<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<body>
<?php
session_start();
//edits the comment of a user on a certain story

//page for editing a comment and updating in the comment database using the already set session vars for story_id and user_id
$_SESSION['comment_id'] = $_POST['comment_id'];
?>
    <form action =edit_comment_sql.php method="post">

	<h2>
		<label for=Comment>New Text for Comment:</label>
		<input type=text name=Comment id=Comment />
		<input type= "hidden" name= "token" value= "<?php echo $_SESSION['token'];?>"/> 
    </h2>
    <p>
		<input type=submit value=Apply />
		<input type=reset />
		<input type= "hidden" name= "token" value= "<?php echo $_SESSION['token'];?>"/> 
	</p>
</form>

<form action="story_viewing.php">
    <p>
		<input type="submit" value="Return to story" />
	</p>
</form>

</body>
</html>

