<html>
<head>
    <title>CPE 580 Final Project</title>
</head>
<body>
<h1>CPE 580 Final Project</h1>
<?php
// Variables
$Temperature = $_GET['temperature'] ?? null;
$Switches = $_GET['switches'] ?? null;
$Temperature2 = $_GET['temperature2'] ?? null;
$Switches2 = $_GET['switches2'] ?? null;
$file = 'output.html';
$output = file_exists($file) ? file_get_contents($file) : '';
// if temperature and switches are not null it is board 1
if ($Temperature !== null && $Switches !== null) {
    $write1 = "<p>Temperature (Board 1): {$Temperature} Celsius</p>";
    $write1 .= "<p>Switches (Board 1): {$Switches}</p>";
    $pattern1 = '/<p>Temperature \(Board 1\):.*?<\/p>\s*<p>Switches \(Board 1\):.*?<\/p>/';
    //regex above is to match the temperature and switches of the first board. If found, replace it with the new content.
    if (preg_match($pattern1, $output)) {
        $output = preg_replace($pattern1, $write1, $output);
    } 
        //otherwise, concatonate output with $write1
        else {
        	$output .= $write1;
    	}
}

// if temperature and switches are not null it is board 2
if ($Temperature2 !== null && $Switches2 !== null) {
    $write2 = "<p>Temperature (Board 2): {$Temperature2} Celsius</p>";
    $write2 .= "<p>Switches (Board 2): {$Switches2}</p>";
    $pattern2 = '/<p>Temperature \(Board 2\):.*?<\/p>\s*<p>Switches \(Board 2\):.*?<\/p>/';
    //regex above is to match the temperature and switches of the second board. If found, replace it with the new content.
    if (preg_match($pattern2, $output)) {
        $output = preg_replace($pattern2, $write2, $output);
    }
    //otherwise, concatonate output with $write2
     else {
        	$output .= $write2;
    }
}
//output the contents to the website.
file_put_contents($file,$output,LOCK_EX);
// Display the content
if (file_exists($file)) {
    echo file_get_contents($file);
}
//Should not happen, but always good to be paranoid.
else {
    echo "<p>Data is unavailable.</p>";
}
        
        
?>
</body>
</html>