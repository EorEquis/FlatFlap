'tabs=4
' --------------------------------------------------------------------------------
' ASCOM CoverCalibrator driver for FlatFlap
'
' Description:	Extension of the ScopeCover project, to allow cover to also
'				use an EL Panel for flats
'
' Implements:	ASCOM CoverCalibrator interface version: 1.0
' Author:		(eor) eeyore@stuffupthere.com
'
' Edit Log:
'
' Date			Who	Vers	Description
' -----------	---	-----	-------------------------------------------------------
' 2022-02-10	eor	1.0.0	Initial edit, from CoverCalibrator template
' ---------------------------------------------------------------------------------
'
'
' Your driver's ID is ASCOM.FlatFlap.CoverCalibrator
'
' The Guid attribute sets the CLSID for ASCOM.DeviceName.CoverCalibrator
' The ClassInterface/None attribute prevents an empty interface called
' _CoverCalibrator from being created and used as the [default] interface
'

' This definition is used to select code that's only applicable for one device type
#Const Device = "CoverCalibrator"

Imports System
Imports System.Collections
Imports System.Collections.Generic
Imports System.Globalization
Imports System.Runtime.InteropServices
Imports System.Text
Imports ASCOM
Imports ASCOM.Astrometry
Imports ASCOM.Astrometry.AstroUtils
Imports ASCOM.DeviceInterface
Imports ASCOM.Utilities

<Guid("014562d5-b244-4c53-96e8-a2f6b6dac11e")>
<ClassInterface(ClassInterfaceType.None)>
Public Class CoverCalibrator

    ' The Guid attribute sets the CLSID for ASCOM.FlatFlap.CoverCalibrator
    ' The ClassInterface/None attribute prevents an empty interface called
    ' _FlatFlap from being created and used as the [default] interface

    ' TODO Replace the not implemented exceptions with code to implement the function or
    ' throw the appropriate ASCOM exception.
    '
    Implements ICoverCalibratorV1

    '
    ' Driver ID and descriptive string that shows in the Chooser
    '
    Friend Shared driverID As String = "ASCOM.FlatFlap.CoverCalibrator"
    Private Shared driverDescription As String = "FlatFlap CoverCalibrator"

    Friend Shared comPortProfileName As String = "COM Port" 'Constants used for Profile persistence
    Friend Shared traceStateProfileName As String = "Trace Level"
    Friend Shared comPortDefault As String = "COM1"
    Friend Shared traceStateDefault As String = "False"

    Friend Shared comPort As String ' Variables to hold the current device configuration
    Friend Shared traceState As Boolean

    Private connectedState As Boolean ' Private variable to hold the connected state
    Private utilities As Util ' Private variable to hold an ASCOM Utilities object
    Private astroUtilities As AstroUtils ' Private variable to hold an AstroUtils object to provide the Range method
    Private TL As TraceLogger ' Private variable to hold the trace logger object (creates a diagnostic log file with information that you specify)
    Private objSerial As ASCOM.Utilities.Serial ' Serial object for sending commands to arduino
    Private statusTimer As System.Timers.Timer
    Private response As String, returnValue As String   ' Variables to hold responses from device, and return them to calling function
    Private cover As CoverStatus, light As CalibratorStatus, lightlevel As Integer



    '
    ' Constructor - Must be public for COM registration!
    '
    Public Sub New()

        ReadProfile() ' Read device configuration from the ASCOM Profile store
        TL = New TraceLogger("", "FlatFlap")
        TL.Enabled = traceState
        TL.LogMessage("CoverCalibrator", "Starting initialisation")

        connectedState = False ' Initialise connected to false
        utilities = New Util() ' Initialise util object
        astroUtilities = New AstroUtils 'Initialise new astro utilities object

        ' Timer to proect the arduino from hyperactive polling (looijng at you, SGP)
        statusTimer = New Timers.Timer
        statusTimer.Interval = 3000
        statusTimer.AutoReset = True
        AddHandler statusTimer.Elapsed, AddressOf Timer_Tick

        TL.LogMessage("CoverCalibrator", "Completed initialisation")
    End Sub

    '
    ' PUBLIC COM INTERFACE ICoverCalibratorV1 IMPLEMENTATION
    '

#Region "Common properties and methods"
    ''' <summary>
    ''' Displays the Setup Dialog form.
    ''' If the user clicks the OK button to dismiss the form, then
    ''' the new settings are saved, otherwise the old values are reloaded.
    ''' THIS IS THE ONLY PLACE WHERE SHOWING USER INTERFACE IS ALLOWED!
    ''' </summary>
    Public Sub SetupDialog() Implements ICoverCalibratorV1.SetupDialog
        ' consider only showing the setup dialog if not connected
        ' or call a different dialog if connected
        If IsConnected Then
            System.Windows.Forms.MessageBox.Show("Already connected, just press OK")
        End If

        Using F As SetupDialogForm = New SetupDialogForm()
            Dim result As System.Windows.Forms.DialogResult = F.ShowDialog()
            If result = DialogResult.OK Then
                WriteProfile() ' Persist device configuration values to the ASCOM Profile store
            End If
        End Using
    End Sub

    Public ReadOnly Property SupportedActions() As ArrayList Implements ICoverCalibratorV1.SupportedActions
        Get
            TL.LogMessage("SupportedActions Get", "Returning empty arraylist")
            Return New ArrayList()
        End Get
    End Property

    Public Function Action(ByVal ActionName As String, ByVal ActionParameters As String) As String Implements ICoverCalibratorV1.Action
        Throw New ActionNotImplementedException("Action " & ActionName & " is not supported by this driver")
    End Function

    Public Sub CommandBlind(ByVal Command As String, Optional ByVal Raw As Boolean = False) Implements ICoverCalibratorV1.CommandBlind
        CheckConnected("CommandBlind")
        ' All commands in the V2 protocol return a response
        Throw New MethodNotImplementedException("CommandBlind")
    End Sub

    Public Function CommandBool(ByVal Command As String, Optional ByVal Raw As Boolean = False) As Boolean _
        Implements ICoverCalibratorV1.CommandBool
        CheckConnected("CommandBool")
        ' TODO The optional CommandBool method should either be implemented OR throw a MethodNotImplementedException
        ' If implemented, CommandBool must send the supplied command to the mount, wait for a response and parse this to return a True Or False value

        ' Dim retString as String = CommandString(command, raw) ' Send the command And wait for the response
        ' Dim retBool as Boolean = XXXXXXXXXXXXX ' Parse the returned string And create a boolean True / False value
        ' Return retBool ' Return the boolean value to the client

        Throw New MethodNotImplementedException("CommandBool")
    End Function

    Public Function CommandString(ByVal Command As String, Optional ByVal Raw As Boolean = False) As String _
        Implements ICoverCalibratorV1.CommandString
        CheckConnected("CommandString")
        objSerial.Transmit(Command + "#")      ' All commands terminated with #
        response = objSerial.ReceiveTerminated("*")
        response = response.Replace("*", "")
        Return response
    End Function

    Public Property Connected() As Boolean Implements ICoverCalibratorV1.Connected
        Get
            TL.LogMessage("Connected Get", IsConnected.ToString())
            Return IsConnected
        End Get
        Set(value As Boolean)
            TL.LogMessage("Connected Set", value.ToString())
            If value = IsConnected Then
                Return
            End If

            If value Then
                Try
                    TL.LogMessage("Connected Set", "Connecting to port " + comPort)
                    objSerial = New ASCOM.Utilities.Serial
                    objSerial.PortName = comPort
                    objSerial.Speed = SerialSpeed.ps9600
                    objSerial.Connected = True
                    connectedState = True
                    ' Either throw a 10uf capacitor between the arduino's GND and RST pins to disable reset on serial connect, or
                    ' wait(2000)
                    returnValue = CommandString("P000")
                    If returnValue = "P999" Then
                        TL.LogMessage("Connected Set", "Connected")
                        getStatus()
                        statusTimer.Enabled = True
                    Else
                        connectedState = False
                        TL.LogMessage("Connected Set", returnValue)
                    End If
                Catch ex As Exception
                    TL.LogMessage("Connected Set Failed", "ERROR : " + ex.Message)
                    connectedState = False
                End Try
            Else
                TL.LogMessage("Connected Set", "Disconnecting from port " + comPort)
                objSerial.Connected = False
                connectedState = False
                statusTimer.Enabled = False
            End If
        End Set
    End Property

    Public ReadOnly Property Description As String Implements ICoverCalibratorV1.Description
        Get
            ' this pattern seems to be needed to allow a public property to return a private field
            Dim d As String = driverDescription
            TL.LogMessage("Description Get", d)
            Return d
        End Get
    End Property

    Public ReadOnly Property DriverInfo As String Implements ICoverCalibratorV1.DriverInfo
        Get
            Dim m_version As Version = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version
            ' TODO customise this driver description
            Dim s_driverInfo As String = "CoverCalibrator driver for FlatFlap. Version: " + m_version.Major.ToString() + "." + m_version.Minor.ToString()
            TL.LogMessage("DriverInfo Get", s_driverInfo)
            Return s_driverInfo
        End Get
    End Property

    Public ReadOnly Property DriverVersion() As String Implements ICoverCalibratorV1.DriverVersion
        Get
            ' Get our own assembly and report its version number
            TL.LogMessage("DriverVersion Get", Reflection.Assembly.GetExecutingAssembly.GetName.Version.ToString(2))
            Return Reflection.Assembly.GetExecutingAssembly.GetName.Version.ToString(2)
        End Get
    End Property

    Public ReadOnly Property InterfaceVersion() As Short Implements ICoverCalibratorV1.InterfaceVersion
        Get
            TL.LogMessage("InterfaceVersion Get", "1")
            Return 1
        End Get
    End Property

    Public ReadOnly Property Name As String Implements ICoverCalibratorV1.Name
        Get
            Dim s_name As String = "ASCOM FlatFlap CoverCalibrator"
            TL.LogMessage("Name Get", s_name)
            Return s_name
        End Get
    End Property

    Public Sub Dispose() Implements ICoverCalibratorV1.Dispose
        ' Clean up the trace logger and util objects
        TL.Enabled = False
        TL.Dispose()
        TL = Nothing
        utilities.Dispose()
        utilities = Nothing
        astroUtilities.Dispose()
        astroUtilities = Nothing
    End Sub

#End Region

#Region "ICoverCalibrator Implementation"

    ''' <summary>
    ''' Returns the state of the device cover, if present, otherwise returns "NotPresent"
    ''' </summary>
    Public ReadOnly Property CoverState() As CoverStatus Implements ICoverCalibratorV1.CoverState
        Get
            TL.LogMessage("CoverState Get", cover.ToString)
            Return cover
        End Get
    End Property

    ''' <summary>
    ''' Initiates cover opening if a cover is present
    ''' </summary>
    Public Sub OpenCover() Implements ICoverCalibratorV1.OpenCover
        TL.LogMessage("OpenCover", "Opening Cover")
        returnValue = CommandString("C00" & CStr(CoverStatus.Open))
        cover = CInt(returnValue.Substring(2, 1))       ' Device returns cover and calibrator statuses w/ same enum as ascom
    End Sub

    ''' <summary>
    ''' Initiates cover closing if a cover is present
    ''' </summary>
    Public Sub CloseCover() Implements ICoverCalibratorV1.CloseCover
        TL.LogMessage("OpenCover", "Closing Cover")
        returnValue = CommandString("C00" & CStr(CoverStatus.Closed))
        cover = CInt(returnValue.Substring(2, 1))       ' Device returns cover and calibrator statuses w/ same enum as ascom
    End Sub

    ''' <summary>
    ''' Stops any cover movement that may be in progress if a cover is present and cover movement can be interrupted.
    ''' </summary>
    Public Sub HaltCover() Implements ICoverCalibratorV1.HaltCover
        ' WTF Conform :  "CoverStatus indicates that the device has cover capability and a MethodNotImplementedException exception was thrown, this method must function per the ASCOM specification."
        ' Bullshit.  Your specification says
        ' "Stops any cover movement that may be in progress if a cover is present and cover movement can be interrupted."
        ' And specifically states a MNIE may be thrown "When CoverState returns NotPresent or if cover movement cannot be interrupted."
        ' Fine.  Here's your cover halting code.
        TL.LogMessage("HaltCover", "Fine we actually halted the cover")
        returnValue = CommandString("C00" & CStr(CoverStatus.Unknown))  ' is unknown an acceptable state after a halt, ASCOM?
        cover = CInt(returnValue.Substring(2, 1))       ' Device returns cover and calibrator statuses w/ same enum as ascom
    End Sub

    ''' <summary>
    ''' Returns the state of the calibration device, if present, otherwise returns "NotPresent"
    ''' </summary>
    Public ReadOnly Property CalibratorState() As CalibratorStatus Implements ICoverCalibratorV1.CalibratorState
        Get
            TL.LogMessage("CalibratorState Get", light.ToString)
            Return light
        End Get
    End Property

    ''' <summary>
    ''' Returns the current calibrator brightness in the range 0 (completely off) to <see cref="MaxBrightness"/> (fully on)
    ''' </summary>
    Public ReadOnly Property Brightness As Integer Implements ICoverCalibratorV1.Brightness
        Get
            ' Here we go actually tracking a brightness of 0 instead of just "off" because Conform and the
            ' specification disagree
            TL.LogMessage("Brightness Get", lightlevel)
            Return lightlevel
        End Get
    End Property

    ''' <summary>
    ''' The Brightness value that makes the calibrator deliver its maximum illumination.
    ''' </summary>
    Public ReadOnly Property MaxBrightness As Integer Implements ICoverCalibratorV1.MaxBrightness
        Get
            TL.LogMessage("MaxBrightness Get", 1)
            Return 1   ' A value of 1 indicates that the calibrator can only be "off" or "on".
        End Get
    End Property

    ''' <summary>
    ''' Turns the calibrator on at the specified brightness if the device has calibration capability
    ''' </summary>
    ''' <param name="Brightness"></param>
    Public Sub CalibratorOn(Brightness As Integer) Implements ICoverCalibratorV1.CalibratorOn
        TL.LogMessage("CalibratorOn", "Light on brightness " & Brightness.ToString)
        Validate("CalibratorOn", Brightness)
        If Brightness = 0 Then
            returnValue = CommandString("L00" & CStr(CalibratorStatus.Off))
            ' An ugly hard coded hack because again conform insists on a condition opposite the expressly defined conditions in the ascom specification
            ' In this case, despite defining a brightness of 0 as "fully off", and despite there being no way to set any brightness above 0 except
            ' by turning the calibrator on, conform says setting brightness to 0 should not turn the calibrator off, but rather leave it
            ' as "Ready" 
            light = 3
        Else
            returnValue = CommandString("L00" & CStr(CalibratorStatus.Ready))
            light = CInt(returnValue.Substring(2, 1))       ' Device returns cover and calibrator statuses w/ same enum as ascom
        End If
        lightlevel = Brightness
    End Sub

    ''' <summary>
    ''' Turns the calibrator off if the device has calibration capability
    ''' </summary>
    Public Sub CalibratorOff() Implements ICoverCalibratorV1.CalibratorOff
        TL.LogMessage("CalibratorOff", "Setting light off")
        returnValue = CommandString("L00" & CStr(CalibratorStatus.Off))
        light = CInt(returnValue.Substring(2, 1))       ' Device returns cover and calibrator statuses w/ same enum as ascom
        lightlevel = 0 ' Because fuck you, that's why
    End Sub

#End Region

#Region "Private properties and methods"
    ' here are some useful properties and methods that can be used as required
    ' to help with

#Region "ASCOM Registration"

    Private Shared Sub RegUnregASCOM(ByVal bRegister As Boolean)

        Using P As New Profile() With {.DeviceType = "CoverCalibrator"}
            If bRegister Then
                P.Register(driverID, driverDescription)
            Else
                P.Unregister(driverID)
            End If
        End Using

    End Sub

    <ComRegisterFunction()>
    Public Shared Sub RegisterASCOM(ByVal T As Type)

        RegUnregASCOM(True)

    End Sub

    <ComUnregisterFunction()>
    Public Shared Sub UnregisterASCOM(ByVal T As Type)

        RegUnregASCOM(False)

    End Sub

#End Region

    ''' <summary>
    ''' Returns true if there is a valid connection to the driver hardware
    ''' </summary>
    Private ReadOnly Property IsConnected As Boolean
        Get
            ' TODO check that the driver hardware connection exists and is connected to the hardware
            Return connectedState
        End Get
    End Property

    ''' <summary>
    ''' Use this function to throw an exception if we aren't connected to the hardware
    ''' </summary>
    ''' <param name="message"></param>
    Private Sub CheckConnected(ByVal message As String)
        If Not IsConnected Then
            Throw New NotConnectedException(message)
        End If
    End Sub

    ''' <summary>
    ''' Read the device configuration from the ASCOM Profile store
    ''' </summary>
    Friend Sub ReadProfile()
        Using driverProfile As New Profile()
            driverProfile.DeviceType = "CoverCalibrator"
            traceState = Convert.ToBoolean(driverProfile.GetValue(driverID, traceStateProfileName, String.Empty, traceStateDefault))
            comPort = driverProfile.GetValue(driverID, comPortProfileName, String.Empty, comPortDefault)
        End Using
    End Sub

    ''' <summary>
    ''' Write the device configuration to the  ASCOM  Profile store
    ''' </summary>
    Friend Sub WriteProfile()
        Using driverProfile As New Profile()
            driverProfile.DeviceType = "CoverCalibrator"
            driverProfile.WriteValue(driverID, traceStateProfileName, traceState.ToString())
            driverProfile.WriteValue(driverID, comPortProfileName, comPort.ToString())
        End Using

    End Sub

#End Region

#Region "My Functions and methods"

    Private Sub Timer_Tick(source As Object, e As EventArgs)
        getStatus()
    End Sub

    Private Sub wait(ByVal interval As Integer)
        '  Delays interval milliseconds, without blocking the UI
        Dim sw As New Stopwatch
        sw.Start()
        Do While sw.ElapsedMilliseconds < interval
            Application.DoEvents()
        Loop
        sw.Stop()
    End Sub

    Private Sub getStatus()
        returnValue = CommandString("S000")
        ' Expect response as S0lc where l is light status, and c is cover status
        light = CInt(returnValue.Substring(2, 1))       ' Device returns cover and calibrator statuses w/ same enum as ascom
        cover = CInt(returnValue.Substring(3, 1))       ' Device returns cover and calibrator statuses w/ same enum as ascom
    End Sub

#End Region

#Region "Validation Functions"

    ''' <summary>
    ''' Checks that the brightness requested is within range, throws invalid value exception if it is not.
    ''' </summary>
    ''' <param name="message">The message.</param>
    ''' <param name="id">The id.</param>
    Private Sub Validate(message As String, value As Integer)
        If (value < 0 Or value > MaxBrightness) Then
            Throw New ASCOM.InvalidValueException(message, value.ToString(), String.Format("0 to {0}", MaxBrightness))
        End If
    End Sub

#End Region
End Class
