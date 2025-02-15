const express = require('express');
const fs = require('fs');
const path = require('path');
const NodeWebcam = require("node-webcam");
const fetch = (...args) => import('node-fetch').then(({ default: fetch }) => fetch(...args));

const app = express();
const port = 3000;

const S3_BUCKET_NAME = "image-captures-sarah";
const S3_BUCKET_URL = `https://${S3_BUCKET_NAME}.s3.amazonaws.com`;

const capturesDir = path.join(__dirname, 'captures');
if (!fs.existsSync(capturesDir)) {
    fs.mkdirSync(capturesDir);
}

// Helper function to get the next file number
const getNextFileNumber = () => {
    const files = fs.readdirSync(capturesDir)
        .filter(file => file.endsWith('.jpeg'))
        .map(file => parseInt(file.replace('image_', '').replace('.jpeg', ''), 10))
        .filter(num => !isNaN(num))
        .sort((a, b) => a - b);

    return files.length > 0 ? files[files.length - 1] + 1 : 1;
};

const opts = {
    width: 1280,
    height: 720,
    quality: 100,
    delay: 0,
    saveShots: true,
    output: "jpeg",
    device: false,
    callbackReturn: "buffer",
    verbose: false
};

const Webcam = NodeWebcam.create(opts);

app.get('/capture', async (req, res) => {
    try {
        const fileNumber = getNextFileNumber();
        const fileName = `image_${fileNumber}.jpeg`;
        const filePath = path.join(capturesDir, fileName);

        const buffer = await new Promise((resolve, reject) => {
            Webcam.capture(filePath, (err, buf) => {
                if (err) reject(err);
                else resolve(buf);
            });
        });

        const response = await fetch(`${S3_BUCKET_URL}/${fileName}`, {
            method: 'PUT',
            body: buffer,
            headers: {
                'Content-Type': 'image/jpeg',
            }
        });

        if (!response.ok) {
            const errorBody = await response.text(); // Capture error response
            console.error(`Failed to upload image to S3. Status: ${response.status} - ${response.statusText}`);
            console.error(`Error Body: ${errorBody}`);
            throw new Error(`Failed to upload image to S3. Status: ${response.status} - ${response.statusText}`);
        }

        res.send(`Image saved as ${fileName} and uploaded. URL: ${S3_BUCKET_URL}/${fileName}`);
    } catch (error) {
        console.error('An error occurred:', error); // Log the full error details
        res.status(500).send(`An error occurred: ${error.message}`);
    }
});

app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
    console.log(`Visit http://localhost:${port}/capture to take a picture`);
});
