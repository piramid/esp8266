<html>
<head>
<meta http-equiv="refresh" content="3">
</head> 
<body>
<?php
$servername = "localhost";
$username = "rongkham";
$password = "q3Ny2is51M";
$dbname = "rongkham_iot";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT * FROM esp8266 ORDER by idS DESC LIMIT 10";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
	echo "<table border='1'><th>ID</th><th>STRANGE</th><th>DATETIME</th>";
    while($row = $result->fetch_assoc()) {
		echo "<tr>";
		echo "<td>".$row['idS']."</td>";
		echo "<td>".$row['strange']."</td>";
		echo "<td>".$row['day']."</td>";
		echo "</tr>";
    }
	echo "</table>";
} else {
    echo "0 results";
}
$conn->close();
?>
</body>
</html>