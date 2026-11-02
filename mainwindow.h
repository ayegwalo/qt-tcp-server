
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Provides the basic functionality for creating the main application window.
#include <QMainWindow>

// QTcpServer allows our application to listen for incoming TCP connections.
#include <QTcpServer>

// QTcpSocket allows our application to communicate with a connected client.
#include <QTcpSocket>


// These are forward declarations.
// They tell the compiler that these Qt classes exist.
// Their full definitions can be included in the .cpp file when needed.
class QLineEdit;       // A field where the user can enter text.
class QPushButton;     // A clickable button.
class QPlainTextEdit;  // A text area that can display multiple lines.


/*
 * MainWindow represents the main window of our TCP server application.
 *
 * It inherits from QMainWindow, which provides standard application-window
 * functionality, such as a central area for displaying widgets.
 */
class MainWindow : public QMainWindow
{
    // Enables Qt's signals and slots system.
    // Signals and slots allow objects to communicate when events occur.
    Q_OBJECT

public:
    // Constructor: runs when a MainWindow object is created.
    //
    // QWidget *parent is an optional parent widget.
    // nullptr means that no parent widget is provided by default.
    explicit MainWindow(QWidget *parent = nullptr);

    // Destructor: runs when the MainWindow object is destroyed.
    // override indicates that this destructor overrides the base-class destructor.
    ~MainWindow() override;


private slots:

    // Starts or stops the TCP server when the user clicks the relevant button.
    void toggleServer();

    // Handles a new connection when a client tries to connect to the server.
    void acceptConnection();

    // Sends a message from the server application to the connected client.
    void sendMessage();

    // Reads and displays data received from the connected client.
    void readIncomingData();


private:

    // QTcpServer listens for incoming TCP connection requests.
    QTcpServer *server;

    // QTcpSocket represents the connection to a client.
    // Through this socket, the server can send data to and receive data
    // from the connected client.
    QTcpSocket *clientSocket;


    // Text field for entering the server address or port information.
    QLineEdit *addressEdit;

    // Button used to start or stop the server.
    QPushButton *connectButton;

    // Displays server status messages and communication logs.
    QPlainTextEdit *outputEdit;

    // Text field where the user types a message to send to the client.
    QLineEdit *messageEdit;

    // Button used to send the typed message to the client.
    QPushButton *sendButton;


    // Helper function that displays a message in the output log.
    //
    // For example:
    // appendLog("Server started.");
    //
    // Using a helper function avoids repeating the same logging code
    // throughout the application.
    void appendLog(const QString &message);
};

// End of the header guard.
// These header guards prevent this header file from being included
// multiple times in the same compilation unit.
#endif // MAINWINDOW_H
