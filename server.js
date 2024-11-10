const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const path = require('path');

const app = express();
const port = 3000;

// Serve static files from the current directory
app.use(express.static(__dirname));

// Use bodyParser middleware to parse JSON requests
app.use(bodyParser.json());

// Route to handle the form submission
app.post('/processCapacity', (req, res) => {
  const capacity = req.body.capacity;

  if (!capacity) {
    return res.status(400).json({ error: 'Capacity is required' });
  }

  // Set up command based on OS
  const isWindows = process.platform === "win32";
  const command = isWindows ? `backend.exe ${capacity}` : `./backend ${capacity}`;

  exec(command, (error, stdout, stderr) => {
    if (error) {
      console.error(`Error executing C++ code: ${error.message}`);
      return res.status(500).json({ error: 'Internal Server Error' });
    }
    if (stderr) {
      console.error(`stderr: ${stderr}`);
      return res.status(500).json({ error: 'Error in C++ code' });
    }

    console.log("Raw C++ output:", stdout);

    const outputs = stdout.trim().split('\n');
    console.log("Parsed outputs:", outputs);

    if (outputs.length === 3) {
      // Convert the outputs to floating point numbers
      res.json({
        output1: parseFloat(outputs[0]).toFixed(2),
        output2: parseFloat(outputs[1]).toFixed(2),
        output3: parseFloat(outputs[2]).toFixed(2)
      });
    } else {
      res.status(500).json({ error: 'Invalid output from C++ program' });
    }
  });
});

// Start the server
app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});
