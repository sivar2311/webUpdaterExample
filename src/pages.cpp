#include "pages.h"

const char* update_GET = R"(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Firmware Upload</title>
</head>

<body>
  <form method="post" enctype="multipart/form-data" action="/update">
    <input name="filename" type="file" size="50" accept=".bin"><br>
    <button>Upload</button>
  </form>
</body>

</html>
)";

const char* update_POST = R"(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Upload finished</title>
</head>

<body>
  Upload finished.<br>
  Please wait until the ESP reboots.<br>
  <p><a href="/update">Go back</a></p>
</body>

</html>
)";
