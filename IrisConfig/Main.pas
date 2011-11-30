unit Main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, SDL, Registry, jpeg, ExtCtrls, Buttons, Unit_XMLNode, FileCtrl,
  ShellAPI;

type
  TConfig = class (TObject)
      private
        FXMLFile: TXMLFile;
        FUOPATH: String;
        FAOS: Boolean;

        FServer: String;
        FPort: Integer;
        FLogin: String;

        FWidth, FHeight: Integer;
        FBPP: Integer;
        FFullScreen: Boolean;

        FMusic: Boolean;
        FSound: Boolean;
        FMP3: Boolean;

        FEmulator: Integer;
      public
        property UOPath: String read FUOPath write FUOPath;
        property AOS: Boolean read FAOS write FAOS;

        property Server: String read FServer write FServer;
        property Login: String read FLogin write FLogin;
        property Port: Integer read FPort write FPort;

        property Width: Integer read FWidth write FWidth;
        property Height: Integer read FHeight write FHeight;
        property BPP: Integer read FBPP write FBPP;
        property FullScreen: Boolean read FFullScreen write FFullScreen;

        property Music: Boolean read FMusic write FMusic;
        property Sound: Boolean read FSound write FSound;
        property MP3: Boolean read FMP3 write FMP3;

        property Emulator: Integer read FEmulator write FEmulator;

        constructor Create (FileName: String);
        destructor Destroy; override;

        procedure CreateXML (FileName: String);
  end;

  TMainForm = class(TForm)
    Button1: TButton;
    Edit1: TEdit;
    Edit2: TEdit;
    Image1: TImage;
    Label1: TLabel;
    ComboBox1: TComboBox;
    Label2: TLabel;
    CheckBox1: TCheckBox;
    Button2: TButton;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    Label3: TLabel;
    Edit3: TEdit;
    Label4: TLabel;
    Edit4: TEdit;
    Label5: TLabel;
    ComboBox2: TComboBox;
    CheckBox2: TCheckBox;
    Label6: TLabel;
    CheckBox3: TCheckBox;
    CheckBox4: TCheckBox;
    Edit5: TEdit;
    Label7: TLabel;
    BitBtn1: TBitBtn;
    BitBtn3: TBitBtn;
    BitBtn2: TBitBtn;
    procedure Button1Click(Sender: TObject);
    procedure BitBtn3Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure BitBtn2Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private-Deklarationen }
    FConfig: TConfig;
    FAskForSaving: Boolean;
    procedure SaveConfig;
  public
    { Public-Deklarationen }
  end;

var
  MainForm: TMainForm;

implementation

{$R *.dfm}

function GetUODir: String;
var
  Registry: TRegistry;
begin
  result := '';

  try
     Registry := TRegistry.Create;
     try
        Registry.RootKey := HKEY_LOCAL_MACHINE;
        if Registry.OpenKey('\SOFTWARE\Origin Worlds Online\Ultima Online Third Dawn\1.0\', false) then begin
          if Registry.ValueExists ('ExePath') then
              result := ExtractFilePath (Registry.ReadString('ExePath'));
        end;
     finally
        Registry.Free;
     end;

  except
  end;

end;

procedure TMainForm.Button1Click(Sender: TObject);
var
  isAOS: Boolean;
begin
  Edit1.Text := GetUODir;
  isAOS := FileExists (Edit1.Text + 'cliloc.enu');
  RadioButton1.Checked := not isAOS;
  RadioButton2.Checked := isAOS;
  Checkbox4.Checked := isAOS;
end;

procedure TMainForm.BitBtn3Click(Sender: TObject);
begin
   FAskForSaving := False;
   Close;
end;

constructor TConfig.Create (FileName: String);
var
  AStream: TStream;
  Node: TXMLNode;
  isSphere, isSphere55R, isPOL, isRunUO, isUOX3: Boolean;
begin
  FServer := 'localhost';
  FPort := 2593;

  FWidth := 1024;
  FHeight := 768;
  FBPP := 32;
  FFullScreen := True;

  FEmulator := 0;
  FAOS := True;

  try
    AStream := TFileStream.Create (FileName, fmOpenRead);
    try
       FXMLFile := TXMLFile.Create(AStream, AStream.Size);
    finally
      AStream.Free;
    end;
  except
    FXMLFile.Free;
    AStream := TMemoryStream.Create;
    try
      FXMLFile := TXMLFile.Create (AStream, AStream.Size);
    finally
      AStream.Free;
    end;
  end;

  if Assigned (FXMLFile.RootNode) then begin
      Node := FXMLFile.RootNode.FINDCHILD ('UO');
      if Assigned (Node) then begin
          FUOPath := Node.GetChildValue('MULPATH');
          FAOS := StrToIntDef(Node.GetChildValue('AOS'), 1) <> 0;
      end;


      Node := FXMLFile.RootNode.FINDCHILD ('NET');
      if Assigned (Node) then begin
          FServer := Node.GetChildValue('SERVER');
          FPort := StrToIntDef(Node.GetChildValue('PORT'), 2593);
          FLogin := Node.GetChildValue('LOGIN');
          isRunUO := StrToIntDef(Node.GetChildValue('IS_RUNUO'), 0) <> 0;
          isSphere := StrToIntDef(Node.GetChildValue('IS_SPHERE'), 0) <> 0;
          isSphere55R := StrToIntDef(Node.GetChildValue('IS_SPHERE55R'), 0) <> 0;
          isPOL := StrToIntDef(Node.GetChildValue('IS_POL'), 0) <> 0;
          isUOX3 := StrToIntDef(Node.GetChildValue('IS_UOX3'), 0) <> 0;

          FEmulator := 5; // Find out emu types (Default: "Other");
          if isRunUO and not isPOL and not isSphere and not isSphere55R and not isUOX3 then
            FEmulator := 0; // RunUO
          if not isRunUO and not isPOL and isSphere and not isSphere55R and not isUOX3  then
            FEmulator := 1; // Sphere Classic
          if not isRunUO and not isPOL and isSphere55R and not isUOX3  then
            FEmulator := 2; // Sphere Revision
          if not isRunUO and isPOL and not isSphere and not isSphere55R and not isUOX3  then
            FEmulator := 3; // POL
          if not isRunUO and not isPOL and not isSphere and not isSphere55R and isUOX3  then
            FEmulator := 4; // UOX3
      end;

      Node := FXMLFile.RootNode.FINDCHILD ('GFX');
      if Assigned (Node) then begin
          FWidth := StrToIntDef(Node.GetChildValue('WIDTH'), 1024);
          FHeight := StrToIntDef(Node.GetChildValue('HEIGHT'), 768);
          FBPP := StrToIntDef(Node.GetChildValue('BPP'), 32);
          FFullScreen := StrToIntDef(Node.GetChildValue('FULLSCREEN'), 1) <> 0;
      end;

      Node := FXMLFile.RootNode.FINDCHILD ('SOUND');
      if Assigned (Node) then begin
          FSound := StrToIntDef(Node.GetChildValue('SOUND'), 0) <> 0;
          FMusic := StrToIntDef(Node.GetChildValue('MUSIC'), 0) <> 0;
          FMP3 := StrToIntDef(Node.GetChildValue('MP3'), 0) <> 0;
      end;
  end;

end;

destructor TConfig.Destroy;
begin
  FXMLFile.Free;
end;

procedure TConfig.CreateXML (FileName: String);
var
  Node: TXMLNode;
  Strings: TStrings;
begin
  Node := FXMLFile.RootNode.AddChild('UO');
  Node.SetChildValue('MULPATH', UOPath);
  Node.SetChildValue('AOS', IntToStr(Ord(AOS)));

  Node := FXMLFile.RootNode.AddChild('NET');
  Node.SetChildValue('SERVER', Server);
  Node.SetChildValue('PORT', IntToStr(Port));
  Node.SetChildValue('LOGIN', Login);
  if Emulator <> 5 then begin // not Other
    Node.SetChildValue('IS_RUNUO', IntToStr(Ord(Emulator = 0)));
    Node.SetChildValue('IS_SPHERE', IntToStr(Ord((Emulator = 1) or (Emulator = 2))));
    Node.SetChildValue('IS_SPHERE55R', IntToStr(Ord(Emulator = 2)));
    Node.SetChildValue('IS_POL', IntToStr(Ord(Emulator = 3)));
    Node.SetChildValue('IS_UOX3', IntToStr(Ord(Emulator = 4)));
  end;

  Node := FXMLFile.RootNode.AddChild('GFX');
  Node.SetChildValue('WIDTH', IntToStr(Width));
  Node.SetChildValue('HEIGHT', IntToStr(Height));
  Node.SetChildValue('BPP', IntToStr(BPP));
  Node.SetChildValue('FULLSCREEN', IntToStr(Ord(FullScreen)));

  Node := FXMLFile.RootNode.AddChild('SOUND');
  Node.SetChildValue('SOUND', IntToStr(Ord(Sound)));
  Node.SetChildValue('MUSIC', IntToStr(Ord(Music)));
  Node.SetChildValue('MP3', IntToStr(Ord(MP3)));

  FXMLFile.RootNode.Section := 'CONFIG';

  Strings := TStringList.Create;
  try
    Strings.Add ('<?xml version="1.0" encoding="ISO-8859-1"?>');
    Strings.Add ('');
    Strings.Add ('<!--');
    Strings.Add ('	Ultima Iris Configuration File - Generated by IrisEdit');
    Strings.Add ('-->');
    Strings.Add ('');

    FXMLFile.RootNode.Save(Strings, 0);
    Strings.SaveToFile(FileName);
  finally
    Strings.Free;
  end;

end;

procedure TMainForm.FormCreate(Sender: TObject);
var
  Modes: PPSDL_Rect;
  Info: PSDL_VideoInfo;
  Index: Integer;
  ResID: Cardinal;
begin
   FConfig := TConfig.Create (ExtractFilePath(Application.ExeName) + 'xml\config.xml');

   SDL_Init (SDL_INIT_VIDEO);
   Modes := SDL_ListModes (nil, SDL_FULLSCREEN or SDL_HWSURFACE);
   if Cardinal(Modes) > 1 then begin
      While Assigned(Modes^) do begin
           Combobox1.Items.AddObject(Format('%d x %d', [ Modes^^.W, Modes^^.H]), Pointer (Cardinal(Modes^^.W) * 65536 + Cardinal (Modes^^.H)));
           inc (Modes);
      end;
  end;
  Info := SDL_GetVideoInfo ();
  Edit2.Text := Format ('%d bits', [Info.vfmt.BitsPerPixel]);

  FConfig.BPP := Info.vfmt.BitsPerPixel;

  SDL_Quit ();

  ResID := FConfig.Width * 65536 + FConfig.Height;
  For Index := 0 to Combobox1.Items.Count - 1 do
    if Cardinal(Combobox1.Items.Objects[Index]) = ResID then
      Combobox1.ItemIndex := Index;

   Edit1.Text := FConfig.UOPath;
   RadioButton1.Checked := not FConfig.AOS;
   RadioButton2.Checked := FConfig.AOS;
   Edit3.Text := FConfig.Server;
   Edit4.Text := IntToStr(FConfig.Port);
   Edit5.Text := FConfig.Login;

   Checkbox1.Checked := FConfig.FullScreen;
   Checkbox2.Checked := FConfig.Sound;
   Checkbox3.Checked := FConfig.Music;
   Checkbox4.Checked := FConfig.MP3;

   Combobox2.ItemIndex := FConfig.Emulator;

   if FConfig.UOPath = '' then
     Button1Click (Sender);
   FAskForSaving := True;
end;

procedure TMainForm.FormDestroy(Sender: TObject);
begin
   FConfig.Free;
end;


procedure TMainForm.Button2Click(Sender: TObject);
var
  isAOS: Boolean;
  Directory: String;
begin
  Directory := Edit1.Text;
  if SelectDirectory ('Please Select your Ultima Online Directory', '', Directory) then begin
      Edit1.Text := Directory;
      isAOS := FileExists (Edit1.Text + 'cliloc.enu');
      RadioButton1.Checked := not isAOS;
      RadioButton2.Checked := isAOS;
  end;
end;

procedure TMainForm.BitBtn1Click(Sender: TObject);
begin
   SaveConfig;

   if FileExists (ExtractFilePath(Application.ExeName) + 'Iris.Exe') then begin
      ShellExecute (0, 'open', 'Iris.exe', nil, PChar(ExtractFilePath(Application.ExeName)), SW_SHOW);
      FAskForSaving := False;
      Close;
   end else
      MessageDlg ('Game executable not found!', mtError, [mbOK], 0);
end;

procedure TMainForm.SaveConfig;
var
  ResID: Cardinal;
begin
   FConfig.UOPath := Edit1.Text;
   FConfig.AOS := RadioButton2.Checked;
   FConfig.Server := Edit3.Text;
   FConfig.Port := StrToIntDef(Edit4.Text, 2593);
   FConfig.Login := Edit5.Text;

   FConfig.FullScreen := Checkbox1.Checked;
   FConfig.Sound := Checkbox2.Checked;
   FConfig.Music := Checkbox3.Checked;
   FConfig.MP3 := Checkbox4.Checked;

   if Combobox1.ItemIndex <> -1 then begin
       ResID := Cardinal(Combobox1.Items.Objects[Combobox1.ItemIndex]);
       FConfig.Width := ResID div 65536;
       FConfig.Height := ResID mod 65536;
   end;

   if Combobox2.ItemIndex <> -1 then
       FConfig.Emulator := Combobox2.ItemIndex;
   FConfig.CreateXML (ExtractFilePath(Application.ExeName) + 'xml\config.xml');
end;

procedure TMainForm.BitBtn2Click(Sender: TObject);
begin
  FAskForSaving := False;
  SaveConfig;
  Close;
end;

procedure TMainForm.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  if FAskForSaving then
   if MessageDlg ('Save changes?', mtConfirmation, [mbYes, mbNo], 0) = mrYes then
      SaveConfig;
end;

end.
