const char css[] PROGMEM = R"rawliteral(
    html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
    }
    h1 {
    font-size: 2.5rem;
    color: white;
    }
    /* Reduce font size for smaller screens */
    @media (max-width: 700px) {
        h1 {
            font-size: 1.8rem; /* Adjust as needed */
        }
    }

    @media (max-width: 400px) {
        h1 {
            font-size: 1.5rem; /* Further reduce for very small screens */
        }
    }
        h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
    }
    h3{
    font-size: 1rem;
    font-weight: bold;
    color: #143642;
    }
    .topnav {
    overflow: hidden;
    background-color: #004990;
    }
    body {
    margin: 0;
    }
.content {
    display: flex;
    justify-content: center;
    height: auto;
    margin-top: 20Px;
}
    .tab {
      overflow: hidden;
      border: 1px solid #ccc;
      background-color: #f1f1f1;
    }
    .tab button {
      background-color: inherit;
      float: left;
      border: none;
      outline: none;
      cursor: pointer;
      padding: 14px 16px;
      transition: 0.3s;
    }
    .tab button:hover {
      background-color: #ddd;
    }
    .tab button.active {
      background-color: #ccc;
    }
    .tabcontent {
      display: none;
      padding: 6px 12px;
      margin-top: 20px;
      border: 1px solid #ccc;
      border-top: none;
    }
    #log {
      border: 1px solid #ccc;
      padding: 10px;
      height: 300px;
      overflow-y: scroll;
    }
    .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding: 20px;
    width: 400px;
    text-align: center;
    margin: 0;
    max-width: 420px;
    min-width: 340px;
    flex: 1 1 340px;
    }

/* Row Layout */
.row {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 20px;
}

.row.center {
    justify-content: center;
}

/* Column Layout */
.column {
    display: flex;
    flex-direction: column;
    align-items: center;
    flex: 1;
    margin: 0 10px;
}

.column label {
    margin-bottom: 5px;
    font-size: 14px;
    font-weight: bold;
}

.column input {
    width: 80px;
    padding: 5px;
    text-align: center;
}

    .button {
    width: 200px;
    padding: 15px 20px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #004990;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
    }
    
    .button_small {
    width: 125px;
    padding: 13px 10px;
    font-size: 16px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #004990;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
    }
    
    .button:hover {
      box-shadow: rgba(50, 50, 93, 0.25) 0px 13px 27px -5px, rgba(0, 0, 0, 0.3) 0px 8px 16px -8px;
    }
    .button:active {
     background-color: #004990;
     box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
     transform: translateY(2px);
    }
    .button_small:hover {
      box-shadow: rgba(50, 50, 93, 0.25) 0px 13px 27px -5px, rgba(0, 0, 0, 0.3) 0px 8px 16px -8px;
    }
    .button_small:active {
     background-color: #004990;
     box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
     transform: translateY(2px);
    }
    .state {
    font-size: 1.5rem;
    color:#8c8c8c;
    font-weight: bold;
    }
    
    input {
        width: 50px;
        border-width: 1px;
        border-radius: 3px;
    }
    
    label {
        display:block;
        float: right;
        padding: 8px; 
    }
    
    .dot {
      height: 30px;
      width: 50px;
      background-color:rgb(220, 0, 0);
      border-radius: 20px;
      display: inline-block;
    }
    
    input_slider {
        width: 0;
    }
    .switch {
      position: relative;
      width: 60px;
      height: 29px;
      display: inline-block;
      float: none;
      padding: 0px; 
      padding-right: 0px; 
      padding-right: 0px;
    }
    
    .switch input { 
      opacity: 0;
      width: 0;
      height: 0;
    }
    
    .switch:hover {
      box-shadow: rgba(50, 50, 93, 0.25) 0px 13px 27px -5px, rgba(0, 0, 0, 0.3) 0px 8px 16px -8px;
    }
    .switch:active {
     background-color: #004990;
     box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
     transform: translateY(2px);
     border-radius: 10%;
    }
    
    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      -webkit-transition: .4s;
      transition: .4s;
    }
    
    .slider:before {
      position: absolute;
      content: "";
      width: 26px;
      left: 4px;
      top: 4px;
      bottom: 4px;
      background-color: white;
      -webkit-transition: .4s;
      transition: .4s;
    }
    
    input:checked + .slider {
      background-color: #004990;
    }
    
    input:focus + .slider {
      box-shadow: 0 0 1px #2196F3;
    }
    
    input:checked + .slider:before {
      -webkit-transform: translateX(26px);
      -ms-transform: translateX(26px);
      transform: translateX(26px);
    }
    
    /* Rounded sliders */
    .slider.round {
      border-radius: 10%;
    }
    
    .slider.round:before {
      border-radius: 10%;
    }

    /* Progress Bar Container */
    .progress-container {
        width: 100%;
        background-color: #ccc;
        border-radius: 5px;
        overflow: hidden;
        height: 20px;
        margin-top: 20px;
        box-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.1);
    }

    /* Progress Bar */
    .progress-bar {
        height: 100%;
        width: 0%;
        background-color: #004990;
        transition: width 0.02s linear, background-color 0.2s linear; /* Smooth transitions */
    }

    /* Fixed Button Bars */
    .top-button-bar {
      position: fixed;
      top: 0;
      left: 0;
      right: 0;
      background: #F8F7F9;;
      padding: 10px;
      z-index: 1000;
      text-align: center;
      box-shadow: 0 2px 5px rgba(0,0,0,0.2);
    }

    .bottom-button-bar {
      position: fixed;
      bottom: 0;
      left: 0;
      right: 0;
      background: #F8F7F9;;
      padding: 10px;
      z-index: 1000;
      text-align: center;
      box-shadow: 0 -2px 5px rgba(0,0,0,0.2);
    }

    /* Add top margin to main content to account for fixed header */
    #mainContent {
      margin-top: 80px;
      margin-bottom: 80px;
      padding: 20px;
    }

    /* Register Cards Layout */
    #registerCards {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      align-items: flex-start;
      gap: 24px;
      width: 100%;
      margin: 0 auto;
      padding: 0;
    }

    /* Bit Container Styles */
    .bit-container {
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      margin-top: 20px;
    }
    
    .bit {
      display: flex;
      flex-direction: column;
      align-items: center;
      margin: 5px;
      width: 40px;
    }
    
    .bit-label {
      font-size: 0.8rem;
      color: #555;
    }
    
    .bit-value input {
      width: 24px;
      text-align: center;
      font-size: 1.2rem;
      font-weight: bold;
      color: #000;
      border: 1px solid #ccc;
      border-radius: 4px;
    }
    
    /* Hex Edit Input */
    .hex-edit {
      font-size: 1.2rem;
      font-family: monospace;
      width: 120px;
      text-align: center;
      margin: 10px auto;
      display: block;
      border: 1px solid #ccc;
      border-radius: 4px;
      padding: 4px;
    }
    
    /* Write Button */
    .write-btn {
      margin: 10px auto;
      display: block;
      padding: 8px 16px;
      font-size: 1rem;
      background: #004990;
      color: #fff;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }
    
    .write-btn:hover {
      background: #143642;
    }

    /* Reset Button Styling */
    .reset-button {
      background-color: #DC3545 !important;
      margin-right: 20px; /* Changed from margin-left to margin-right */
    }

    .reset-button:hover {
      background-color: #C82333 !important;
    }

    /* Simple Reset Overlay */
    #resetOverlay {
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: rgba(0, 0, 0, 0.7);
      backdrop-filter: blur(10px);
      display: none;
      justify-content: center;
      align-items: center;
      z-index: 10000;
      font-family: Arial, sans-serif;
    }

    .reset-container {
      background: white;
      padding: 40px;
      border-radius: 10px;
      text-align: center;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
      max-width: 400px;
    }

    .reset-title {
      margin: 0 0 20px 0;
      font-size: 24px;
      color: #333;
      font-weight: normal;
    }

    .reset-message {
      margin: 0 0 20px 0;
      font-size: 16px;
      color: #666;
    }

    .countdown-timer {
      font-size: 48px;
      font-weight: bold;
      color: #DC3545;
      margin: 20px 0;
    }

    .reset-submessage {
      margin: 0;
      font-size: 14px;
      color: #999;
    }

    /* Connection Overlay Animations */
    @keyframes bloodDrip {
      0% { 
        filter: brightness(1) contrast(1);
        transform: scale(1);
      }
      50% {
        filter: brightness(1.2) contrast(1.1);
        transform: scale(1.15);
      }
      100% { 
        filter: brightness(0.8) contrast(1.2);
        transform: scale(1);
      }
    }
    
    @keyframes drip {
      0% {
        top: -100px;
        opacity: 0;
      }
      10% {
        opacity: 0.7;
      }
      90% {
        opacity: 0.7;
      }
      100% {
        top: 100vh;
        opacity: 0;
      }
    }
    
    @keyframes bloodPulse {
      0%, 100% { opacity: 0.3; }
      50% { opacity: 0.6; }
    }
)rawliteral";

const char html_bitview[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Register Viewer</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>

<!-- Top Button Bar -->
<div class="top-button-bar">
  <button class="button_small" onclick="addRegisterCard()">Add Register</button>
  <button class="button_small" onclick="readAllRegisters()">Read All</button>
</div>

<div id="mainContent">
  <div id="registerCards"></div>
</div>

<!-- Bottom Button Bar -->
<div class="bottom-button-bar">
  <button class="button_small reset-button" onclick="confirmReset()">Reset Device</button>
  <button class="button_small" onclick="saveProfile()">Save Profile</button>
  <button class="button_small" onclick="loadProfile()">Load Profile</button>
  <button class="button_small" onclick="exportProfile()">Export Profile</button>
  <input type="file" id="importProfileInput" style="display:none" accept=".json" onchange="importProfile(event)">
  <button class="button_small" onclick="document.getElementById('importProfileInput').click()">Import Profile</button>
  <button class="button_small" onclick="showFileManager()">CSV Files</button>
</div>

<!-- File Manager Overlay -->
<div id="fileManagerOverlay" style="display:none; position:fixed; top:0; left:0; width:100%; height:100%; background:rgba(0,0,0,0.8); z-index:1000;">
  <div style="position:absolute; top:50%; left:50%; transform:translate(-50%,-50%); background:white; padding:20px; border-radius:10px; max-width:80%; max-height:80%; overflow:auto;">
    <h2>CSV Files</h2>
    <div id="fileList">Loading files...</div>
    <button class="button" onclick="hideFileManager()" style="margin-top:20px;">Close</button>
  </div>
</div>

<!-- Simple Reset Overlay -->
<div id="resetOverlay">
  <div class="reset-container">
    <h2 class="reset-title">Device Reset</h2>
    <p class="reset-message">Teensy is rebooting...</p>
    <div class="countdown-timer" id="countdownTimer">10</div>
    <p class="reset-submessage">Please wait while the device restarts</p>
  </div>
</div>

<script>
var gateway = `ws://${window.location.hostname}:8080`;
var websocket;
var registerCards = [];
var cardIdCounter = 0;

// Add a flag to track when we're in reset mode
var isResetting = false;

window.addEventListener('load', onLoad);

function onLoad() {
  initWebSocket();
  loadCardsFromStorage();
}

// Simple reset functions
function confirmReset() {
  if (confirm("Are you sure you want to reset the device?")) {
    isResetting = true; // Set reset flag
    showResetOverlay();
    fetch('/reset')
      .then(response => {
        console.log('Reset command sent');
      })
      .catch(error => {
        console.error('Reset request failed:', error);
        isResetting = false; // Clear flag on error
        hideResetOverlay();
        alert('Failed to send reset command');
      });
  }
}

function showResetOverlay() {
  const overlay = document.getElementById('resetOverlay');
  overlay.style.display = 'flex';
  
  // Start countdown
  let countdown = 10;
  const timer = setInterval(() => {
    countdown--;
    const timerElement = document.getElementById('countdownTimer');
    if (timerElement) {
      timerElement.textContent = countdown;
      if (countdown <= 0) {
        timerElement.textContent = "0";
        timerElement.style.color = "#28a745";
        document.querySelector('.reset-message').textContent = "Reconnecting...";
        clearInterval(timer);
        
        setTimeout(() => {
          window.location.reload();
        }, 2000);
      }
    }
  }, 1000);
  
  // Auto-reload after 10 seconds regardless
  setTimeout(() => {
    window.location.reload();
  }, 10000);
}

function hideResetOverlay() {
  const overlay = document.getElementById('resetOverlay');
  if (overlay) {
    overlay.style.display = 'none';
  }
}

// Connection overlay - kept as dynamic creation since it's more complex
function showOminousOverlay() {
  let overlay = document.getElementById('connectionOverlay');
  if (!overlay) {
    overlay = document.createElement('div');
    overlay.id = 'connectionOverlay';
    overlay.style.cssText = `
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: rgba(139, 0, 0, 0.4);
      backdrop-filter: blur(4px);
      display: flex;
      justify-content: center;
      align-items: center;
      z-index: 9999;
      font-size: 4rem;
      color: #FF0000;
      text-shadow: 
        -2px -2px 0 #000000,
        2px -2px 0 #000000,
        -2px 2px 0 #000000,
        2px 2px 0 #000000,
        0 0 10px #FF0000,
        0 0 20px #FF0000,
        0 0 30px #FF0000,
        3px 3px 0px #000000,
        6px 6px 0px #330000;
      font-family: Impact, Arial Black, sans-serif;
      font-weight: bold;
      animation: bloodDrip 2s ease-in-out infinite alternate;
    `;
    
    overlay.innerHTML = `
      <div style="
        z-index: 10000; 
        position: relative;
        border: 4px solid #000000;
        background: rgba(0, 0, 0, 0.8);
        padding: 20px 40px;
        border-radius: 10px;
        box-shadow: 
          0 0 20px rgba(255, 0, 0, 0.8),
          inset 0 0 20px rgba(139, 0, 0, 0.3);
      ">WASTED!</div>
    `;
    
    // Create blood drips
    for (let i = 0; i < 12; i++) {
      const drop = document.createElement('div');
      drop.style.cssText = `
        position: absolute;
        width: ${Math.random() * 15 + 8}px;
        height: ${Math.random() * 80 + 40}px;
        background: linear-gradient(180deg, #8B0000, #DC143C, #8B0000);
        border-radius: 0 0 50% 50%;
        left: ${Math.random() * 100}%;
        top: -100px;
        animation: drip ${Math.random() * 3 + 2}s linear infinite;
        animation-delay: ${Math.random() * 2}s;
        opacity: 0.7;
        z-index: 9998;
      `;
      overlay.appendChild(drop);
    }
    
    document.body.appendChild(overlay);
  }
  overlay.style.display = 'flex';
}

function hideOminousOverlay() {
  const overlay = document.getElementById('connectionOverlay');
  if (overlay) {
    overlay.style.display = 'none';
  }
}

// Update the initWebSocket function's onopen handler
function initWebSocket() {
  websocket = new WebSocket(gateway);
  
  // Clear any existing intervals
  if (websocket.heartbeatInterval) clearInterval(websocket.heartbeatInterval);
  if (websocket.connectionCheckInterval) clearInterval(websocket.connectionCheckInterval);
  if (websocket.pingTimeout) clearTimeout(websocket.pingTimeout);
  
  websocket.onopen = () => {
    hideOminousOverlay();
    console.log('WebSocket connection opened');
    
    // Check if this is a reconnection (overlay was shown)
    const overlay = document.getElementById('connectionOverlay');
    if (overlay && overlay.style.display === 'flex') {
      console.log('Connection re-established - refreshing page');
      // Small delay to ensure connection is stable before refresh
      setTimeout(() => {
        window.location.reload();
      }, 500);
      return;
    }
    
    // Clear reset flag when connection is restored
    isResetting = false;
    
    websocket.isAlive = true;
    startConnectionMonitoring();
    
    // Restore streaming states after connection is established
    restoreStreamingStates();
  };
  
  websocket.onclose = (event) => {
    console.log('WebSocket connection closed, code:', event.code, 'reason:', event.reason);
    websocket.isAlive = false;
    
    // Don't show ominous overlay if we're resetting
    if (!isResetting) {
      showOminousOverlay();
    }
    
    cleanupIntervals();
    setTimeout(initWebSocket, 500);
  };
  
  websocket.onerror = (error) => {
    console.log('WebSocket error:', error);
    websocket.isAlive = false;
    
    // Don't show ominous overlay if we're resetting
    if (!isResetting) {
      showOminousOverlay();
    }
    
    cleanupIntervals();
  };
  
  websocket.onmessage = (event) => {
    websocket.isAlive = true;
    if (event.data === 'pong') {
      if (websocket.pingTimeout) clearTimeout(websocket.pingTimeout);
      console.log('Received pong - connection alive');
    } else {
      onMessage(event);
    }
  };
}

function startConnectionMonitoring() {
  // Ultra-aggressive ping every 200ms
  websocket.heartbeatInterval = setInterval(() => {
    if (websocket.readyState === WebSocket.OPEN && websocket.isAlive) {
      websocket.isAlive = false; // Will be set to true when pong received
      websocket.send('ping');
      
      // If no pong received within 300ms, consider connection dead
      websocket.pingTimeout = setTimeout(() => {
        if (!websocket.isAlive) {
          console.log('Ping timeout - connection appears dead');
          websocket.close();
          // Don't show ominous overlay if we're resetting
          if (!isResetting) {
            showOminousOverlay();
          }
        }
      }, 300);
      
    } else if (websocket.readyState !== WebSocket.OPEN) {
      // Don't show ominous overlay if we're resetting
      if (!isResetting) {
        showOminousOverlay();
      }
      cleanupIntervals();
    }
  }, 200); // Ping every 200ms
  
  // Additional state monitoring every 100ms
  websocket.connectionCheckInterval = setInterval(() => {
    if (websocket.readyState === WebSocket.CLOSED || websocket.readyState === WebSocket.CLOSING) {
      // Don't show ominous overlay if we're resetting
      if (!isResetting) {
        showOminousOverlay();
      }
      cleanupIntervals();
    }
  }, 100); // Check every 100ms
}

function cleanupIntervals() {
  if (websocket.heartbeatInterval) {
    clearInterval(websocket.heartbeatInterval);
    websocket.heartbeatInterval = null;
  }
  if (websocket.connectionCheckInterval) {
    clearInterval(websocket.connectionCheckInterval);
    websocket.connectionCheckInterval = null;
  }
  if (websocket.pingTimeout) {
    clearTimeout(websocket.pingTimeout);
    websocket.pingTimeout = null;
  }
}

function getProfileData() {
  // Save all relevant card data (id, name, address, bits, streaming)
  return registerCards.map(card => ({
    id: card.id,
    name: document.getElementById(`registerName_${card.id}`)?.value || card.name || "",
    address: document.getElementById(`registerAddress_${card.id}`)?.value || card.address || "",
    bits: card.bits,
    streaming: document.getElementById(`streamCheckbox_${card.id}`)?.checked || card.streaming || false,  // Save checkbox state
    bitWidth: card.bitWidth || 32  // Save bit width in profile
  }));
}

function setProfileData(profile) {
  registerCards = profile.map((card, idx) => ({
    id: card.id !== undefined ? card.id : idx,
    name: card.name || "",
    address: card.address || "",
    bits: Array(card.bitWidth || 32).fill(0),
    streaming: card.streaming || false,  // Restore streaming status from profile
    bitWidth: card.bitWidth || 32  // Restore bit width from profile
  }));
  cardIdCounter = registerCards.length ? Math.max(...registerCards.map(c => c.id)) + 1 : 0;
  renderRegisterCards();
  saveCardsToStorage();
  
  // Ensure streaming checkboxes are properly set after rendering
  setTimeout(() => {
    registerCards.forEach(card => {
      const checkbox = document.getElementById(`streamCheckbox_${card.id}`);
      if (checkbox) {
        checkbox.checked = card.streaming || false;
      }
    });
  }, 100);
}

// Update the restoreStreamingStates function to include bitWidth
function restoreStreamingStates() {
  setTimeout(() => {
    registerCards.forEach(card => {
      if (card.streaming && card.address) {
        console.log(`Restoring stream for card ${card.id}, address: ${card.address}, bitWidth: ${card.bitWidth || 32}`);
        websocket.send(JSON.stringify({
          action: "startStream",
          address: card.address,
          cardId: card.id,
          bitWidth: card.bitWidth || 32
        }));
      }
    });
  }, 1000);
}

// Add the toggleStream function that was missing
function toggleStream(id) {
  const card = registerCards.find(c => c.id === id);
  const checkbox = document.getElementById(`streamCheckbox_${id}`);
  const address = document.getElementById(`registerAddress_${id}`).value;
  
  if (!card) return;
  
  card.streaming = checkbox.checked;
  card.address = address;
  saveCardsToStorage();
  
  if (checkbox.checked && address) {
    console.log(`Starting stream for card ${id}, address: ${address}, bitWidth: ${card.bitWidth || 32}`);
    websocket.send(JSON.stringify({
      action: "startStream",
      address: address,
      cardId: id,
      bitWidth: card.bitWidth || 32
    }));
  } else {
    console.log(`Stopping stream for card ${id}`);
    websocket.send(JSON.stringify({
      action: "stopStream",
      cardId: id
    }));
  }
}

function loadCardsFromStorage() {
  // Always start with just one empty register card, ignore storage
  registerCards = [];
  cardIdCounter = 0;
  addRegisterCard();
}

function saveCardsToStorage() {
  // Save id, name, address, and streaming for persistence
  const saveData = registerCards.map(card => ({
    id: card.id,
    name: card.name || "",
    address: card.address || "",
    streaming: card.streaming || false,
    bitWidth: card.bitWidth || 32  // Save bit width
  }));
  localStorage.setItem("registerCards", JSON.stringify(saveData));
  localStorage.setItem("cardIdCounter", cardIdCounter);
}

function addRegisterCard() {
  const id = cardIdCounter++;
  registerCards.push({
    id,
    name: "",
    address: "",
    bits: Array(32).fill(0),
    streaming: false,
    bitWidth: 32  // Add bit width property
  });
  saveCardsToStorage();
  renderRegisterCards();
}

function removeRegisterCard(id) {
  registerCards = registerCards.filter(card => card.id !== id);
  saveCardsToStorage();
  renderRegisterCards();
}

function renderRegisterCards() {
  const container = document.getElementById("registerCards");
  container.innerHTML = "";
  registerCards.forEach(card => {
    const cardDiv = document.createElement("div");
    cardDiv.className = "card";
    cardDiv.style.marginBottom = "20px";
    cardDiv.innerHTML = `
      <div style="width:100%; display:flex; justify-content:space-between; align-items:center;">
        <input type="text" id="registerName_${card.id}" placeholder="Register Name" value="${card.name || ""}" style="flex:1; margin-right:10px; font-size:1rem; padding:4px; border-radius:4px; border:1px solid #ccc;">
        <button style="float:right" onclick="removeRegisterCard(${card.id})">Remove</button>
      </div>
      <div style="display:flex; align-items:center; justify-content:center; margin-top:10px; gap:10px;">
        <label for="registerAddress_${card.id}" style="margin:0;">Address:</label>
        <input type="text" id="registerAddress_${card.id}" style="width: 120px; height: 30px; font-size: medium;" value="${card.address || ""}" placeholder="0x00000000">
        <select id="bitWidth_${card.id}" style="height: 30px; font-size: medium;" onchange="onBitWidthChange(${card.id})">
          <option value="32" ${(card.bitWidth || 32) === 32 ? 'selected' : ''}>32-bit</option>
          <option value="16" ${(card.bitWidth || 32) === 16 ? 'selected' : ''}>16-bit</option>
          <option value="8" ${(card.bitWidth || 32) === 8 ? 'selected' : ''}>8-bit</option>
        </select>
        <button class="button_small" onclick="requestRegisterValue(${card.id})">Read</button>
      </div>
      <div style="display:flex; align-items:center; justify-content:center; margin-top:10px; gap:10px;">
        <label for="hexValue_${card.id}" style="margin:0;">Hex:</label>
        <input id="hexValue_${card.id}" class="hex-edit" maxlength="10" value="0x00000000" oninput="onHexEdit(${card.id})" />
        <label style="margin:0;">Dec:</label>
        <span id="decValue_${card.id}">0</span>
      </div>
      <div id="registerBinary_${card.id}" class="bit-container" style="margin-top:18px;"></div>
      <div style="display:flex; align-items:center; justify-content:space-between; width:100%; margin-top:18px;">
        <label style="margin:0;">
          <input type="checkbox" id="streamCheckbox_${card.id}" ${card.streaming ? 'checked' : ''} onchange="toggleStream(${card.id})"> Stream
        </label>
        <button class="button_small" onclick="writeRegister(${card.id})">Write Register</button>
      </div>
    `;
    container.appendChild(cardDiv);

    // Name persistence: update card.name on input change
    const nameInput = cardDiv.querySelector(`#registerName_${card.id}`);
    nameInput.addEventListener("input", function() {
      card.name = nameInput.value;
      saveCardsToStorage();
    });

    // Address persistence: update card.address on input change
    const addrInput = cardDiv.querySelector(`#registerAddress_${card.id}`);
    addrInput.addEventListener("input", function() {
      card.address = addrInput.value;
      saveCardsToStorage();
    });

    renderBits(card.id);
  });
}

// Add bit width change handler
function onBitWidthChange(id) {
  const card = registerCards.find(c => c.id === id);
  const bitWidth = parseInt(document.getElementById(`bitWidth_${id}`).value);
  if (card) {
    card.bitWidth = bitWidth;
    card.bits = Array(bitWidth).fill(0);
    saveCardsToStorage();
    renderBits(id);
    updateHexFromBits(id);
  }
}

function requestRegisterValue(id) {
  const card = registerCards.find(c => c.id === id);
  const address = document.getElementById(`registerAddress_${id}`).value;
  const bitWidth = card.bitWidth || 32;
  card.address = address;
  saveCardsToStorage();
  const encodedAddress = encodeURIComponent(address);
  websocket.lastRequestedId = id;
  fetch(`/readRegister?address=${encodedAddress}&bitWidth=${bitWidth}`)
    .then(response => {
      if (!response.ok) {
        // Don't show ominous overlay if we're resetting
        if (!isResetting) {
          showOminousOverlay();
        }
        alert("Failed to send register request.");
      }
    })
    .catch(error => {
      // Don't show ominous overlay if we're resetting
      if (!isResetting) {
        showOminousOverlay();
      }
      console.error('Network error:', error);
    });
}

function writeRegister(id) {
  const card = registerCards.find(c => c.id === id);
  const address = document.getElementById(`registerAddress_${id}`).value;
  const hexValue = document.getElementById(`hexValue_${id}`).value;
  const bitWidth = card.bitWidth || 32;
  const encodedAddress = encodeURIComponent(address);
  const encodedValue = encodeURIComponent(hexValue);
  websocket.lastRequestedId = id;
  fetch(`/writeRegister?address=${encodedAddress}&value=${encodedValue}&bitWidth=${bitWidth}`)
    .then(response => {
      if (!response.ok) {
        // Don't show ominous overlay if we're resetting
        if (!isResetting) {
          showOminousOverlay();
        }
        alert("Failed to write register value.");
      }
    })
    .catch(error => {
      // Don't show ominous overlay if we're resetting
      if (!isResetting) {
        showOminousOverlay();
      }
      console.error('Network error:', error);
    });
}

// Update onMessage to handle streaming updates
function onMessage(event) {
  const data = event.data;
  if (data.startsWith("registerValue:")) {
    const [_, address, value] = data.split(":");
    let id = websocket.lastRequestedId;
    if (typeof id === "undefined") id = 0;
    updateHexAndBits(id, value);
  } else if (data.startsWith("streamValue:")) {
    // Format: streamValue:cardId:value
    const [_, cardId, value] = data.split(":");
    updateHexAndBits(Number(cardId), value);
  }
}

function updateHexAndBits(id, hexValue) {
  const card = registerCards.find(c => c.id === id);
  if (!card) return;
  
  const bitWidth = card.bitWidth || 32;
  let hex = hexValue.replace(/^0x/i, "");
  hex = hex.padStart(Math.ceil(bitWidth / 4), "0");
  document.getElementById(`hexValue_${id}`).value = "0x" + hex.toUpperCase();
  const dec = parseInt(hex, 16);
  document.getElementById(`decValue_${id}`).textContent = dec;
  const binary = dec.toString(2).padStart(bitWidth, '0');
  card.bits = binary.split('').map(Number);
  renderBits(id);
}

function renderBits(id) {
  const card = registerCards.find(c => c.id === id);
  if (!card) return;
  const container = document.getElementById(`registerBinary_${id}`);
  if (!container) return;
  container.innerHTML = "";

  const bitWidth = card.bitWidth || 32;
  const bitsPerRow = bitWidth === 8 ? 8 : 8;
  const numRows = Math.ceil(bitWidth / bitsPerRow);

  for (let rowNum = 0; rowNum < numRows; rowNum++) {
    const row = document.createElement("div");
    row.style.display = "flex";
    row.style.justifyContent = "center";
    row.style.marginBottom = "4px";

    const startBit = rowNum * bitsPerRow;
    const endBit = Math.min(startBit + bitsPerRow, bitWidth);

    for (let col = 0; col < (endBit - startBit); col++) {
      const idx = startBit + col;

      // Create a column for each bit (label above input)
      const bitCol = document.createElement("div");
      bitCol.style.display = "flex";
      bitCol.style.flexDirection = "column";
      bitCol.style.alignItems = "center";

      // Label
      const bitLabel = document.createElement("div");
      bitLabel.style.width = "24px";
      bitLabel.style.textAlign = "center";
      bitLabel.style.fontSize = "0.8rem";
      bitLabel.style.color = "#555";
      bitLabel.textContent = `b${bitWidth - 1 - idx}`;
      bitCol.appendChild(bitLabel);

      // Input
      const bitInput = document.createElement("input");
      bitInput.type = "text";
      bitInput.maxLength = 1;
      bitInput.value = card.bits[idx];
      bitInput.style.width = "24px";
      bitInput.style.textAlign = "center";
      bitInput.style.fontSize = "1.2rem";
      bitInput.style.fontWeight = "bold";
      bitInput.style.color = "#fff";
      bitInput.style.border = "1px solid #ccc";
      bitInput.style.borderRadius = "4px";
      bitInput.style.margin = "4px 0px";
      // Color for 1/0
      bitInput.style.backgroundColor = card.bits[idx] === 1 ? "#225522" : "#661a1a";
      bitInput.oninput = function() {
        if (bitInput.value !== "0" && bitInput.value !== "1") {
          bitInput.value = card.bits[idx];
          return;
        }
        card.bits[idx] = parseInt(bitInput.value, 10);
        renderBits(id); // re-render to update color
        updateHexFromBits(id);
      };
      bitCol.appendChild(bitInput);

      row.appendChild(bitCol);

      // Demarcation after every 4 bits except at the end
      if ((col + 1) % 4 === 0 && col !== (endBit - startBit - 1)) {
        const sep = document.createElement("div");
        sep.style.width = "6px";
        sep.style.borderLeft = "2px solid #bbb";
        sep.style.margin = "0px 0px 0px 6px";
        row.appendChild(sep);
      }
    }
    container.appendChild(row);
  }
}

// Update updateHexFromBits to handle different bit widths
function updateHexFromBits(id) {
  const card = registerCards.find(c => c.id === id);
  if (!card) return;
  const bitWidth = card.bitWidth || 32;
  const binaryStr = card.bits.join('');
  const hex = parseInt(binaryStr, 2).toString(16).toUpperCase().padStart(Math.ceil(bitWidth / 4), "0");
  document.getElementById(`hexValue_${id}`).value = "0x" + hex;
  document.getElementById(`decValue_${id}`).textContent = parseInt(hex, 16);
}

// Update onHexEdit to handle different bit widths
function onHexEdit(id) {
  const card = registerCards.find(c => c.id === id);
  if (!card) return;
  
  const bitWidth = card.bitWidth || 32;
  const maxHexChars = Math.ceil(bitWidth / 4);
  let hex = document.getElementById(`hexValue_${id}`).value.replace(/^0x/i, "");
  
  const hexPattern = new RegExp(`^[0-9a-fA-F]{1,${maxHexChars}}$`);
  if (!hexPattern.test(hex)) return;
  
  hex = hex.padStart(maxHexChars, "0");
  const dec = parseInt(hex, 16);
  document.getElementById(`decValue_${id}`).textContent = dec;
  const binary = dec.toString(2).padStart(bitWidth, '0');
  card.bits = binary.split('').map(Number);
  renderBits(id);
}

// Update saveCardsToStorage and loadCardsFromStorage to include bitWidth
function saveCardsToStorage() {
  const saveData = registerCards.map(card => ({
    id: card.id,
    name: card.name || "",
    address: card.address || "",
    streaming: card.streaming || false,
    bitWidth: card.bitWidth || 32  // Save bit width
  }));
  localStorage.setItem("registerCards", JSON.stringify(saveData));
  localStorage.setItem("cardIdCounter", cardIdCounter);
}

function loadCardsFromStorage() {
  // Always start with just one empty register card, ignore storage
  registerCards = [];
  cardIdCounter = 0;
  addRegisterCard();
}

// Update getProfileData and setProfileData to include bitWidth
function getProfileData() {
  return registerCards.map(card => ({
    id: card.id,
    name: document.getElementById(`registerName_${card.id}`)?.value || card.name || "",
    address: document.getElementById(`registerAddress_${card.id}`)?.value || card.address || "",
    bits: card.bits,
    streaming: document.getElementById(`streamCheckbox_${card.id}`)?.checked || card.streaming || false,
    bitWidth: card.bitWidth || 32  // Save bit width in profile
  }));
}

function setProfileData(profile) {
  registerCards = profile.map((card, idx) => ({
    id: card.id !== undefined ? card.id : idx,
    name: card.name || "",
    address: card.address || "",
    bits: Array(card.bitWidth || 32).fill(0),
    streaming: card.streaming || false,  // Restore streaming status from profile
    bitWidth: card.bitWidth || 32  // Restore bit width from profile
  }));
  cardIdCounter = registerCards.length ? Math.max(...registerCards.map(c => c.id)) + 1 : 0;
  renderRegisterCards();
  saveCardsToStorage();
  
  // Ensure streaming checkboxes are properly set after rendering
  setTimeout(() => {
    registerCards.forEach(card => {
      const checkbox = document.getElementById(`streamCheckbox_${card.id}`);
      if (checkbox) {
        checkbox.checked = card.streaming || false;
      }
    });
  }, 100);
}

// Expose removeRegisterCard to global scope for inline onclick
window.removeRegisterCard = removeRegisterCard;

// Add this new function after the existing functions in the script section

function readAllRegisters() {
  let readCount = 0;
  registerCards.forEach(card => {
    if (card.address && card.address.trim() !== "") {
      // Add a small delay between reads to avoid overwhelming the server
      setTimeout(() => {
        requestRegisterValue(card.id);
      }, readCount * 200); // 100ms delay between each read
      readCount++;
    }
  });
  
  if (readCount === 0) {
    alert("No register cards with addresses found to read.");
  } else {
    console.log(`Reading ${readCount} registers...`);
  }
}

function saveProfile() {
  const profile = getProfileData();
  localStorage.setItem("registerCardsProfile", JSON.stringify(profile));
  alert("Profile saved to local storage!");
}

function loadProfile() {
  const saved = localStorage.getItem("registerCardsProfile");
  if (saved) {
    try {
      const profile = JSON.parse(saved);
      setProfileData(profile);
      
      // Start streaming for cards that have streaming enabled
      setTimeout(() => {
        registerCards.forEach(card => {
          if (card.streaming && card.address) {
            console.log(`Starting stream for loaded card ${card.id}, address: ${card.address}, bitWidth: ${card.bitWidth || 32}`);
            if (websocket && websocket.readyState === WebSocket.OPEN) {
              websocket.send(JSON.stringify({
                action: "startStream",
                address: card.address,
                cardId: card.id,
                bitWidth: card.bitWidth || 32
              }));
            }
          }
        });
      }, 500); // Small delay to ensure cards are rendered
      
      // alert("Profile loaded successfully!");
    } catch (e) {
      alert("Failed to load profile: Invalid data");
    }
  } else {
    alert("No saved profile found");
  }
}

function exportProfile() {
  const profile = getProfileData();
  const dataStr = JSON.stringify(profile, null, 2);
  const dataBlob = new Blob([dataStr], {type: 'application/json'});
  
  const link = document.createElement('a');
  link.href = URL.createObjectURL(dataBlob);
  link.download = 'register_profile.json';
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
}

function importProfile(event) {
  const file = event.target.files[0];
  if (!file) return;
  
  const reader = new FileReader();
  reader.onload = function(e) {
    try {
      const profile = JSON.parse(e.target.result);
      setProfileData(profile);
      // alert("Profile imported successfully!");
    } catch (error) {
      alert("Failed to import profile: Invalid JSON file");
    }
  };
  reader.readAsText(file);
}

// File Management Functions
function showFileManager() {
  document.getElementById('fileManagerOverlay').style.display = 'block';
  loadFileList();
}

function hideFileManager() {
  document.getElementById('fileManagerOverlay').style.display = 'none';
}

function loadFileList() {
  const fileListDiv = document.getElementById('fileList');
  fileListDiv.innerHTML = 'Loading files...';
  
  fetch('/files')
    .then(response => response.json())
    .then(files => {
      if (files.length === 0) {
        fileListDiv.innerHTML = '<p>No CSV files found.</p>';
        return;
      }
      
      let html = '<table style="width:100%; border-collapse:collapse;">';
      html += '<tr style="background:#f0f0f0;"><th style="padding:8px; border:1px solid #ddd;">File Name</th><th style="padding:8px; border:1px solid #ddd;">Size</th><th style="padding:8px; border:1px solid #ddd;">Action</th></tr>';
      
      files.forEach(file => {
        const sizeKB = (file.size / 1024).toFixed(1);
        html += `<tr>
          <td style="padding:8px; border:1px solid #ddd;">${file.name}</td>
          <td style="padding:8px; border:1px solid #ddd;">${sizeKB} KB</td>
          <td style="padding:8px; border:1px solid #ddd;">
            <button class="button_small" onclick="downloadFile('${file.name}')">Download</button>
            <button class="button_small" style="margin-left:5px; background-color:#dc3545;" onclick="deleteFile('${file.name}')">Delete</button>
          </td>
        </tr>`;
      });
      
      html += '</table>';
      fileListDiv.innerHTML = html;
    })
    .catch(error => {
      console.error('Error loading files:', error);
      fileListDiv.innerHTML = '<p>Error loading files.</p>';
    });
}

function downloadFile(fileName) {
  const link = document.createElement('a');
  link.href = `/download?file=${encodeURIComponent(fileName)}`;
  link.download = fileName;
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
}

function deleteFile(fileName) {
  if (confirm(`Are you sure you want to delete "${fileName}"? This action cannot be undone.`)) {
    fetch(`/delete?file=${encodeURIComponent(fileName)}`)
      .then(response => response.json())
      .then(data => {
        if (data.status === 'success') {
          //alert(`File "${fileName}" deleted successfully.`);
          loadFileList(); // Refresh the file list
        } else {
          alert(`Error deleting file: ${data.message}`);
        }
      })
      .catch(error => {
        console.error('Error deleting file:', error);
        alert(`Error deleting file "${fileName}". Please try again.`);
      });
  }
}
</script>
</body>
</html>
)rawliteral";