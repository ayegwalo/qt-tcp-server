
// Include header file.
// It declares the MainWindow class, its functions, and its variables.
#include "mainwindow.h"

// Include the Qt classes needed to build the interface and handle networking.
#include <QLineEdit>        // A field for entering text on one line.
#include <QPushButton>      // A clickable button.
#include <QPlainTextEdit>   // A text area that displays multiple lines.
#include <QHBoxLayout>      // Arranges widgets horizontally, from left to right.
#include <QVBoxLayout>      // Arranges widgets vertically, from top to bottom.
#include <QWidget>          // The base class for most Qt interface elements.
#include <QHostAddress>     // Represents an IP address.
#include <QAbstractSocket>  // Provides socket states and error information.
#include <QTextCursor>      // Allows control of the text cursor.
#include <QDateTime>        // Provides the current date and time.


// ============================================================
// CONSTRUCTOR: CREATE AND INITIALISE THE TCP SERVER WINDOW
// ============================================================

// This function runs when a MainWindow object is created.
//
// QMainWindow(parent) initialises the parent window class.
// server(new QTcpServer(this)) creates the TCP server.
// The 'this' argument makes MainWindow the server's parent,
// so Qt manages the server object's lifetime.
// clientSocket(nullptr) means no client is connected initially.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    server(new QTcpServer(this)),
    clientSocket(nullptr)
{
    // Set the text displayed in the window's title bar.
    setWindowTitle("TCP Server");

    // Set the initial window size: 700 pixels wide and 500 pixels high.
    resize(700, 500);


    // ========================================================
    // 1. CREATE THE USER INTERFACE WIDGETS
    // ========================================================

    // Create a text field where the user enters the port number.
    addressEdit = new QLineEdit(this);

    // Show a hint when the port field is empty.
    addressEdit->setPlaceholderText("Port");

    // Use port 5000 as the default value.
    // The client must connect to the same port.
    addressEdit->setText("5000");


    // Create a button for starting or stopping the server.
    connectButton = new QPushButton("Listen", this);


    // Create a text area to display server status and chat messages.
    outputEdit = new QPlainTextEdit(this);

    // Prevent users from manually changing the output log.
    // The application can still add text to it.
    outputEdit->setReadOnly(true);


    // Create a field where the server operator types messages to send.
    messageEdit = new QLineEdit(this);

    // Display a hint when the message field is empty.
    messageEdit->setPlaceholderText("Enter message...");


    // Create a button for sending messages to the connected client.
    sendButton = new QPushButton("Send", this);

    // Disable the Send button because no client is connected yet.
    sendButton->setEnabled(false);


    // ========================================================
    // 2. ARRANGE THE WIDGETS USING LAYOUTS
    // ========================================================

    // Create a horizontal layout for the port field and Listen button.
    // These two widgets will appear next to each other.
    auto *addressLayout = new QHBoxLayout;

    // Add the port input field to the layout.
    addressLayout->addWidget(addressEdit);

    // Add the Listen button beside the port field.
    addressLayout->addWidget(connectButton);


    // Create a horizontal layout for the message field and Send button.
    auto *messageLayout = new QHBoxLayout;

    // Add the message input field.
    messageLayout->addWidget(messageEdit);

    // Add the Send button beside the message field.
    messageLayout->addWidget(sendButton);


    // Create the main vertical layout.
    // Its contents will be arranged from top to bottom.
    auto *mainLayout = new QVBoxLayout;

    // Place the port field and Listen button at the top.
    mainLayout->addLayout(addressLayout);

    // Place the output log below the top row.
    // The stretch factor of 1 allows the log to expand vertically.
    mainLayout->addWidget(outputEdit, 1);

    // Place the message field and Send button at the bottom.
    mainLayout->addLayout(messageLayout);


    // Create the central widget that holds the entire interface.
    auto *centralWidget = new QWidget(this);

    // Apply the main layout to the central widget.
    centralWidget->setLayout(mainLayout);

    // Tell QMainWindow to display this widget as its main content.
    setCentralWidget(centralWidget);


    // ========================================================
    // 3. CONNECT BUTTON ACTIONS TO FUNCTIONS
    // ========================================================

    // When the Listen button is clicked, call toggleServer().
    // This function starts the server or stops it if it is already running.
    connect(connectButton, &QPushButton::clicked,
            this, &MainWindow::toggleServer);


    // When the Send button is clicked, call sendMessage().
    connect(sendButton, &QPushButton::clicked,
            this, &MainWindow::sendMessage);


    // When the user presses Enter in the message field,
    // send the message without requiring a button click.
    connect(messageEdit, &QLineEdit::returnPressed,
            this, &MainWindow::sendMessage);


    // ========================================================
    // 4. RESPOND TO INCOMING CLIENT CONNECTIONS
    // ========================================================

    // QTcpServer emits newConnection when a client requests a connection
    // and the connection is ready to be accepted.
    //
    // Call acceptConnection() to retrieve and handle that connection.
    connect(server, &QTcpServer::newConnection,
            this, &MainWindow::acceptConnection);


    // Display an initial message to tell the user that the server
    // application is ready to be started.
    appendLog("Server ready. Enter a port and click Listen.");
}


// ============================================================
// DESTRUCTOR: CLEAN UP THE MAIN WINDOW
// ============================================================

// This function runs when the MainWindow object is destroyed.
//
// The destructor is empty because Qt automatically manages child objects.
// The server and widgets have parents, so Qt will delete them when their
// parents are destroyed.
MainWindow::~MainWindow()
{
}


// ============================================================
// FUNCTION: START OR STOP THE SERVER
// ============================================================

void MainWindow::toggleServer()
{
    // Check whether the server is already listening for connections.
    if (server->isListening()) {

        // Stop listening for new client connections.
        server->close();

        // If a client socket exists, request that its connection be closed.
        if (clientSocket) {
            clientSocket->disconnectFromHost();
        }

        // Change the button text to allow the server to be started again.
        connectButton->setText("Listen");

        // Allow the user to change the port number.
        addressEdit->setEnabled(true);

        // Inform the user that the server has stopped.
        appendLog("Server stopped.");

        // Stop executing this function.
        return;
    }


    // ========================================================
    // 5. READ AND VALIDATE THE PORT NUMBER
    // ========================================================

    // This variable records whether the text can be converted into a number.
    bool validPort = false;

    // Read the port number from the input field.
    // trimmed() removes spaces at the beginning and end.
    // toUInt() converts the text into an unsigned integer.
    //
    // For example, the text "5000" becomes the number 5000.
    // validPort becomes true if the conversion succeeds.
    const uint port = addressEdit->text().trimmed().toUInt(&validPort);


    // Check that the port is a valid number between 1 and 65535.
    if (!validPort || port < 1 || port > 65535) {

        // Display an error if the entered port is invalid.
        appendLog("Enter a valid port from 1 to 65535.");

        // Stop because the server cannot listen on an invalid port.
        return;
    }


    // ========================================================
    // 6. START LISTENING FOR CLIENT CONNECTIONS
    // ========================================================

    // Start the TCP server.
    //
    // QHostAddress::AnyIPv4 means the server listens on all available
    // local IPv4 network interfaces, subject to firewall and network
    // configuration.
    //
    // static_cast<quint16>(port) converts the validated port number
    // to the 16-bit unsigned integer type expected by listen().
    //
    // listen() returns true if the server starts listening successfully.
    // Otherwise, it returns false.
    if (!server->listen(QHostAddress::AnyIPv4,
                        static_cast<quint16>(port))) {

        // If the server could not start, display Qt's error description.
        appendLog("Listen error: " + server->errorString());

        // Stop because the server failed to start.
        return;
    }


    // The server has started successfully.
    // Change the button text so the user can stop it later.
    connectButton->setText("Stop Listening");

    // Prevent the port from being changed while the server is listening.
    addressEdit->setEnabled(false);

    // Display the port on which the server is listening.
    appendLog("Listening on port " + QString::number(port));
}


// ============================================================
// FUNCTION: ACCEPT A NEW CLIENT CONNECTION
// ============================================================

void MainWindow::acceptConnection()
{
    // Retrieve the next client connection waiting to be accepted.
    //
    // nextPendingConnection() returns a QTcpSocket pointer.
    // This socket represents the connection to that particular client.
    QTcpSocket *newSocket = server->nextPendingConnection();


    // If no connection is available, there is nothing to process.
    if (!newSocket) {
        return;
    }


    // ========================================================
    // 7. ALLOW ONLY ONE CONNECTED CLIENT
    // ========================================================

    // This example supports one active client at a time.
    //
    // Check whether clientSocket exists and whether that client
    // is still connected.
    if (clientSocket &&
        clientSocket->state() == QAbstractSocket::ConnectedState) {

        // Inform the user that another client is already connected.
        appendLog("A client is already connected; rejecting new client.");

        // Request that the new connection be closed.
        newSocket->disconnectFromHost();

        // Ask Qt to delete the socket later, when it is safe to do so.
        newSocket->deleteLater();

        // Stop because this client will not be accepted.
        return;
    }


    // Store the accepted client's socket in clientSocket.
    // Other functions will use this pointer to send and receive data.
    clientSocket = newSocket;


    // Display the client's IP address and port number.
    //
    // peerAddress() returns the remote client's IP address.
    // peerPort() returns the port number used by that client.
    appendLog("Client connected from "
              + clientSocket->peerAddress().toString()
              + ":" + QString::number(clientSocket->peerPort()));


    // Enable the Send button because a client is now connected.
    sendButton->setEnabled(true);


    // When data arrives from this client, call readIncomingData().
    connect(clientSocket, &QTcpSocket::readyRead,
            this, &MainWindow::readIncomingData);


    // When this client disconnects, run the following lambda function.
    //
    // [this, newSocket] allows the lambda to access MainWindow members
    // and the specific socket that disconnected.
    connect(clientSocket, &QTcpSocket::disconnected,
            this, [this, newSocket]() {

                // Record the disconnection in the server log.
                appendLog("Client disconnected.");


                // Only clear clientSocket if it still points to this connection.
                // This avoids clearing a different socket by mistake.
                if (clientSocket == newSocket) {

                    // There is no active client to communicate with.
                    clientSocket = nullptr;

                    // Disable sending until another client connects.
                    sendButton->setEnabled(false);
                }


                // Schedule the disconnected socket for deletion.
                // Qt will delete it when control returns to the event loop.
                newSocket->deleteLater();
            });


    // If a socket error occurs, display the error description.
    connect(clientSocket, &QTcpSocket::errorOccurred,
            this, [this, newSocket](QAbstractSocket::SocketError) {

                // errorString() gives a human-readable explanation of the error.
                appendLog("Socket error: " + newSocket->errorString());
            });


    // Send a welcome message to the newly connected client.
    //
    // The message ends with CRLF (\r\n), a line-ending convention
    // commonly used by text-based protocols.
    //
    // write() queues the bytes for sending; it does not guarantee
    // that the client has already received them.
    clientSocket->write("Welcome from the Qt server!\r\n");
}


// ============================================================
// FUNCTION: SEND A MESSAGE TO THE CONNECTED CLIENT
// ============================================================

void MainWindow::sendMessage()
{
    // Check that clientSocket exists and represents an active connection.
    if (!clientSocket ||
        clientSocket->state() != QAbstractSocket::ConnectedState) {

        // Inform the user that there is no connected client.
        appendLog("No client connected.");

        // Stop because there is nowhere to send the message.
        return;
    }


    // Read the message from the input field.
    // trimmed() removes spaces at the beginning and end.
    const QString message = messageEdit->text().trimmed();


    // Do not send an empty message.
    if (message.isEmpty()) {
        return;
    }


    // Get the current time in hours:minutes:seconds format.
    // Example: "21:45:08".
    const QString time =
        QDateTime::currentDateTime().toString("HH:mm:ss");


    // Convert the message into UTF-8 bytes and send it to the client.
    //
    // TCP sends bytes, not QString objects.
    // The CRLF ending may help the receiving application identify
    // the end of a text message.
    clientSocket->write((message + "\r\n").toUtf8());


    // Display the outgoing message in the server's log.
    // %1 is replaced by the time and %2 by the message.
    appendLog(QString("[%1] Server: %2").arg(time, message));


    // Clear the input field after the message has been queued for sending.
    messageEdit->clear();
}


// ============================================================
// FUNCTION: READ DATA RECEIVED FROM THE CLIENT
// ============================================================

void MainWindow::readIncomingData()
{
    // Check whether there is a client socket to read from.
    if (!clientSocket) {
        return;
    }


    // Read all bytes currently available in the socket's receive buffer.
    // QByteArray stores raw bytes received over the network.
    const QByteArray data = clientSocket->readAll();


    // Get the current time in hours:minutes:seconds format.
    const QString time =
        QDateTime::currentDateTime().toString("HH:mm:ss");


    // Convert the received UTF-8 bytes into a QString for display.
    const QString message = QString::fromUtf8(data);


    // Convert Windows-style CRLF line endings into ordinary newline characters.
    // This can make the output easier to display consistently.
    //
    // A carriage return by itself is also converted to a newline.
    QString formattedMessage = message;
    formattedMessage.replace("\r\n", "\n");
    formattedMessage.replace("\r", "\n");


    // Add the timestamp, identify the sender as the client,
    // and display the received text in the output area.
    //
    // trimmed() removes whitespace at the beginning and end,
    // including line-ending characters.
    outputEdit->appendPlainText(
        QString("[%1] Client: %2").arg(time, formattedMessage.trimmed())
        );
}


// ============================================================
// FUNCTION: ADD A MESSAGE TO THE SERVER LOG
// ============================================================

void MainWindow::appendLog(const QString &message)
{
    // Add the message as a new line in the output area.
    //
    // The "[Server]" label identifies the message as a server status
    // message or a log entry generated by this application.
    outputEdit->appendPlainText("[Server] " + message);
}