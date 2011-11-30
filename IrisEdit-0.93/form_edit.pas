unit form_edit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Unit_OpenGLWrapper, Unit_ViewPort, Unit_OpenGL, Menus, StdCtrls, ExtCtrls,
  ComCtrls, Unit_Models, Buttons, ToolWin, Unit_Settings, Unit_OpenGLGeometry,
  form_coords, unit_ArtLoader, Unit_IntegerList, contnrs, Unit_Rasterer,
  Unit_TextureManager, Unit_ModelLoader, Form_Scale, ExtDlgs, Decal;

type

  TEditForm = class(TForm)
    MainMenu1: TMainMenu;
    Model1: TMenuItem;
    Options1: TMenuItem;
    BackgroundColor1: TMenuItem;
    Panel1: TPanel;
    Edit1: TEdit;
    Edit2: TEdit;
    PageControl: TPageControl;
    PointTabSheet: TTabSheet;
    Label3: TLabel;
    ViewPoints1: TMenuItem;
    Ground1: TMenuItem;
    Outbox1: TMenuItem;
    Close1: TMenuItem;
    ToolBar1: TToolBar;
    SpeedButton1: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton6: TSpeedButton;
    SpeedButton7: TSpeedButton;
    Background1: TMenuItem;
    Lines1: TMenuItem;
    Points1: TMenuItem;
    Ground2: TMenuItem;
    Edges1: TMenuItem;
    Faces1: TMenuItem;
    N3: TMenuItem;
    Texturing1: TMenuItem;
    N4: TMenuItem;
    ColorDialog: TColorDialog;
    StatusBar: TStatusBar;
    Label1: TLabel;
    Label2: TLabel;
    Edit3: TEdit;
    Edit4: TEdit;
    Edit5: TEdit;
    SpeedButton8: TSpeedButton;
    SpeedButton9: TSpeedButton;
    SpeedButton10: TSpeedButton;
    SpeedButton11: TSpeedButton;
    SpeedButton14: TSpeedButton;
    SpeedButton15: TSpeedButton;
    FaceTabSheet: TTabSheet;
    ModelTabSheet: TTabSheet;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Edit6: TMenuItem;
    CheckBox1: TCheckBox;
    Panel2: TPanel;
    TextureTabSheet: TTabSheet;
    Button6: TButton;
    Button7: TButton;
    Button8: TButton;
    Button9: TButton;
    Button4: TButton;
    Button10: TButton;
    Button11: TButton;
    Button12: TButton;
    Button13: TButton;
    Button14: TButton;
    Button15: TButton;
    GroupBox1: TGroupBox;
    SpeedButton16: TSpeedButton;
    SpeedButton18: TSpeedButton;
    SpeedButton17: TSpeedButton;
    SpeedButton19: TSpeedButton;
    SpeedButton23: TSpeedButton;
    SpeedButton21: TSpeedButton;
    SpeedButton22: TSpeedButton;
    SpeedButton20: TSpeedButton;
    SpeedButton26: TSpeedButton;
    SpeedButton25: TSpeedButton;
    SpeedButton24: TSpeedButton;
    SpeedButton27: TSpeedButton;
    Edit7: TEdit;
    Edit10: TEdit;
    Edit8: TEdit;
    Edit11: TEdit;
    Edit12: TEdit;
    Edit9: TEdit;
    Button5: TButton;
    Button16: TButton;
    Button17: TButton;
    Button19: TButton;
    Button20: TButton;
    Button21: TButton;
    Label4: TLabel;
    Label5: TLabel;
    Button22: TButton;
    Button23: TButton;
    Button24: TButton;
    Button25: TButton;
    Button26: TButton;
    Backfaces1: TMenuItem;
    Button27: TButton;
    SpeedButton28: TSpeedButton;
    Bevel1: TBevel;
    PreviewPaintBox: TPaintBox;
    Copy1: TMenuItem;
    Paste1: TMenuItem;
    Button18: TButton;
    Label6: TLabel;
    Edit13: TEdit;
    Button29: TButton;
    Button30: TButton;
    Button31: TButton;
    ListView1: TListView;
    Button33: TButton;
    LightingTabSheet: TTabSheet;
    CheckBox2: TCheckBox;
    Label7: TLabel;
    Edit14: TEdit;
    SpeedButton29: TSpeedButton;
    SpeedButton30: TSpeedButton;
    SpeedButton31: TSpeedButton;
    SpeedButton32: TSpeedButton;
    SpeedButton33: TSpeedButton;
    Label8: TLabel;
    Edit15: TEdit;
    Label9: TLabel;
    SpeedButton34: TSpeedButton;
    Label10: TLabel;
    Edit17: TEdit;
    Label11: TLabel;
    Shape1: TShape;
    Button28: TButton;
    CheckBox3: TCheckBox;
    Label12: TLabel;
    Edit18: TEdit;
    Edit19: TEdit;
    Label13: TLabel;
    Label14: TLabel;
    Edit20: TEdit;
    Edit16: TEdit;
    Button34: TButton;
    TrackBar1: TTrackBar;
    Label15: TLabel;
    CheckBox4: TCheckBox;
    Button35: TButton;
    Button36: TButton;
    N1: TMenuItem;
    Scale1: TMenuItem;
    ranslate1: TMenuItem;
    Fixandmerge1: TMenuItem;
    Mirror1: TMenuItem;
    X1: TMenuItem;
    Y1: TMenuItem;
    Z1: TMenuItem;
    Rotate1: TMenuItem;
    N901: TMenuItem;
    N1801: TMenuItem;
    N2701: TMenuItem;
    SpeedButton13: TSpeedButton;
    SpeedButton35: TSpeedButton;
    Bevel2: TBevel;
    Button37: TButton;
    Button32: TButton;
    OpenPictureDialog: TOpenPictureDialog;
    Button38: TButton;
    PopupMenu1: TPopupMenu;
    ApplyTexture1: TMenuItem;
    SelectFaces1: TMenuItem;
    CheckBox5: TCheckBox;
    selectallfaces1: TMenuItem;
    unselectallfaces1: TMenuItem;
    selectuntexturedfaces1: TMenuItem;
    N2: TMenuItem;
    CopyLightOptions1: TMenuItem;
    PasteLightOptions1: TMenuItem;
    Import3DS1: TMenuItem;
    N5: TMenuItem;
    OpenDialog3DS: TOpenDialog;
    ParticleTabSheet: TTabSheet;
    CheckBox6: TCheckBox;
    Label16: TLabel;
    Edit21: TEdit;
    SpeedButton12: TSpeedButton;
    SpeedButton36: TSpeedButton;
    SpeedButton37: TSpeedButton;
    SpeedButton38: TSpeedButton;
    SpeedButton39: TSpeedButton;
    SpeedButton40: TSpeedButton;
    Edit22: TEdit;
    Label17: TLabel;
    Label18: TLabel;
    Edit23: TEdit;
    Edit24: TEdit;
    Label19: TLabel;
    CheckBox7: TCheckBox;
    Label20: TLabel;
    Edit25: TEdit;
    Label21: TLabel;
    Label22: TLabel;
    Edit26: TEdit;
    Label23: TLabel;
    procedure FormShow(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure FormMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure SpeedButton1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Background1Click(Sender: TObject);
    procedure FormMouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    procedure ViewPoints1Click(Sender: TObject);
    procedure Edges1Click(Sender: TObject);
    procedure Faces1Click(Sender: TObject);
    procedure Ground1Click(Sender: TObject);
    procedure Outbox1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Edit3Change(Sender: TObject);
    procedure SpeedButton11Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button10Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Backfaces1Click(Sender: TObject);
    procedure Close1Click(Sender: TObject);
    procedure PreviewPaintBoxPaint(Sender: TObject);
    procedure Texturing1Click(Sender: TObject);
    procedure Button18Click(Sender: TObject);
    procedure Button28Click(Sender: TObject);
    procedure CheckBox1Click(Sender: TObject);
    procedure CheckBox2Click(Sender: TObject);
    procedure CheckBox3Click(Sender: TObject);
    procedure Edit14Change(Sender: TObject);
    procedure Edit15Change(Sender: TObject);
    procedure Edit16Change(Sender: TObject);
    procedure Edit17Change(Sender: TObject);
    procedure Edit18Change(Sender: TObject);
    procedure Edit19Change(Sender: TObject);
    procedure Edit20Change(Sender: TObject);
    procedure PageControlChange(Sender: TObject);
    procedure Button34Click(Sender: TObject);
    procedure SpeedButton29Click(Sender: TObject);
    procedure TrackBar1Change(Sender: TObject);
    procedure Edit7Change(Sender: TObject);
    procedure SpeedButton18Click(Sender: TObject);
    procedure Button29Click(Sender: TObject);
    procedure Button30Click(Sender: TObject);
    procedure CheckBox4Click(Sender: TObject);
    procedure Button19Click(Sender: TObject);
    procedure Button35Click(Sender: TObject);
    procedure ranslate1Click(Sender: TObject);
    procedure Button37Click(Sender: TObject);
    procedure Button33Click(Sender: TObject);
    procedure Button32Click(Sender: TObject);
    procedure SelectFaces1Click(Sender: TObject);
    procedure ApplyTexture1Click(Sender: TObject);
    procedure Button38Click(Sender: TObject);
    procedure Button31Click(Sender: TObject);
    procedure selectallfaces1Click(Sender: TObject);
    procedure Edit6Click(Sender: TObject);
    procedure Copy1Click(Sender: TObject);
    procedure CopyLightOptions1Click(Sender: TObject);
    procedure PasteLightOptions1Click(Sender: TObject);
    procedure Paste1Click(Sender: TObject);
    procedure Import3DS1Click(Sender: TObject);
    procedure CheckBox6Click(Sender: TObject);
    procedure CheckBox7Click(Sender: TObject);
    procedure Edit21Change(Sender: TObject);
    procedure Edit22Change(Sender: TObject);
    procedure Edit23Change(Sender: TObject);
    procedure Edit25Change(Sender: TObject);
    procedure Edit26Change(Sender: TObject);
    procedure Edit24Change(Sender: TObject);
    procedure SpeedButton12Click(Sender: TObject);
  private
    { Private-Deklarationen }
    FEngine: TGLEngine;
    FViewPort: TViewPort;
    FModel: TModel;
    FChangeLock: Boolean;
    FMouseDownPos: TPoint;
    FLastMouseUp: TDateTime;
    FTileID: Integer;
    FPreviewFailed: Boolean;
    OpenGLisStarted : boolean;
    FName: String;
    FTextureList: TGLInstanceTextureList;
    FUsedTextures: DMap;
    procedure UpdateFields;
    procedure UpdateStatus;
    procedure AddNodeCallback (AX, AY, AZ: Single);
    procedure ShowSelectedFace;
    procedure ShowTextures;
    procedure ShowLightSettings;
    procedure ShowEffectSettings;
  public
    { Public-Deklarationen }
    constructor Create (AModel: TModel; AID: Integer; AName: String; AOwner: TComponent); reintroduce;
    destructor Destroy; override;
    procedure Render;
  end;

implementation

uses form_TextureManager;
{$R *.DFM}

constructor TEditForm.Create(AModel: TModel; AID: Integer; AName: String; AOwner: TComponent);
begin
   inherited Create (AOwner);
   pModelLoader.LockModel(AID);

   Application.UpdateFormatSettings := False;
   DecimalSeparator := '.';

   Assert(Assigned(AModel));
   FEngine := TGLEngine.Create;
   FModel := AModel;
   FTileID := AID;
   FName := AName;
   Edit1.Text := IntToStr (FTileID);
   Edit2.Text := FName;
   Caption := Format ('Editing #%d (%s)', [ FTileID, FName ]);

   ShowLightSettings;
   ShowEffectSettings;
   FModel.LastChange := Now;

   FTextureList := TGLInstanceTextureList.Create;
   FUsedTextures := DMap.Create;
end;

destructor TEditForm.Destroy;
begin
   pModelLoader.UnLockModel(FTileID);
   FTextureList.Free;
   FTextureList := nil;
   FUsedTextures.Free;
   FUsedTextures := nil;
   FViewPort.Free;
   FViewPort := nil;
   FEngine.Free;
   FEngine := nil;
   inherited Destroy;
end;

procedure TEditForm.FormShow(Sender: TObject);
begin
//   FEngine.StartGL (Canvas.Handle);
//   FEngine.SetupGL;
//   FEngine.SetClearColor (BackgroundColor);
//   FViewPort := TViewPort.Create(0, 0, ClientWidth, ClientHeight);
//   FViewPort.SetOrientation(voIso);
//   ShowSelectedFace;
//   Render;
//   ShowTextures;
end;

procedure TEditForm.Render;
var
  AngleX, AngleY: Integer;
  lX, lY, lZ, l: Single;
begin
    if not Assigned(FViewPort) or not Assigned(FEngine) then exit;

    FEngine.MakeCurrent (Canvas.Handle);
    FViewPort.Apply(FEngine, ClientHeight);

    glClear(GL_DEPTH_BUFFER_BIT or GL_COLOR_BUFFER_BIT);
    glEnable (GL_DEPTH_TEST);
    FViewPort.LoadView;

    glDisable (GL_TEXTURE_2D);
    if Ground1.Checked then
        FModel.RenderGround;

    if Edges1.Checked then
        FModel.RenderWireFrame;

    if Texturing1.Checked then
        glEnable (GL_TEXTURE_2D);
    if Faces1.Checked then
        FModel.RenderFaces (FTextureList);
    if Backfaces1.Checked then
        FModel.RenderBackfaces;
    glDisable (GL_TEXTURE_2D);

    if ViewPoints1.Checked then
        FModel.RenderPoints;
    if Outbox1.Checked then
        FModel.RenderOutbox;

    if (PageControl.ActivePage = LightingTabSheet) and (FModel.FLightSourceOptions.FModelIsLightSource) then begin
        glPushMatrix ();
        glTranslatef (-0.5, -0.5, -FModel.Height / 20);
        glDisable (GL_LIGHTING);
        SetColor (Shape1.Brush.Color);
        glBegin(GL_LINES);
        with FModel.FLightSourceOptions do begin
          For AngleX := 0 to 12 do
            For AngleY := 0 to 12 do begin
              lX := sin (AngleX / 6 * PI);
              lY := sin (AngleY / 6 * PI);
              lZ := cos (AngleX / 6 * PI) + cos (AngleY / 6 * PI);
              l := sqrt(lx * lx + ly * ly + lz * lz);
              glVertex3f(FX, FY, FZ);
              glVertex3f(FX + FRadius * lX / l, FY + FRadius * lY / l, FZ + FRadius * lZ / l);
            end;
{          glVertex3f(FX, FY - FRadius, FZ);
          glVertex3f(FX, FY + FRadius, FZ);
          glVertex3f(FX, FY, FZ - FRadius);
          glVertex3f(FX, FY, FZ + FRadius);}
        end;
        glEnd();
        glEnable (GL_LIGHTING);
        glPopMatrix;
    end;

    if (PageControl.ActivePage = ParticleTabSheet) and (mfModelHasParticleEffect in FModel.Flags) then begin
        glPushMatrix ();
        glTranslatef (-0.5, -0.5, -FModel.Height / 20);
        glTranslatef (FModel.FEffectOptions.FX, FModel.FEffectOptions.FY, FModel.FEffectOptions.FZ);
        if FModel.FEffectOptions.FRotateEffect then begin
            glRotatef (FModel.FEffectOptions.FAngle1, 1, 0, 0);
            glRotatef (FModel.FEffectOptions.FAngle2, 0, 1, 0);
        end;
        glDisable (GL_LIGHTING);
        SetColor (clGreen);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(10, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 10, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 10);
        glEnd();
        glEnable (GL_LIGHTING);
        glPopMatrix;
    end;

    SwapBuffers (Canvas.Handle);
end;

procedure TEditForm.FormPaint(Sender: TObject);
begin
if not OpenGLisStarted then begin
    OpenGLisStarted := True;
   FEngine.StartGL (Canvas.Handle);
   FEngine.SetupGL;
   FEngine.SetClearColor (BackgroundColor);
   FViewPort := TViewPort.Create(0, 0, ClientWidth, ClientHeight);
   FViewPort.SetOrientation(voIso);
   ShowSelectedFace;
   Render;
   ShowTextures;
end
end;

procedure TEditForm.FormResize(Sender: TObject);
begin
   if Assigned(FViewPort) then begin
      FViewPort.Width := Panel1.Left;
      FViewPort.Height := ClientHeight;
   FEngine.StartGL (Canvas.Handle);
   FEngine.SetupGL;
   FEngine.SetClearColor (BackgroundColor);
   FViewPort := TViewPort.Create(0, 0, ClientWidth, ClientHeight);
   FViewPort.SetOrientation(voIso);
   ShowSelectedFace;
   Render;
   ShowTextures;
   end;
end;

procedure TEditForm.FormMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  Handled: Boolean;
begin
   Handled := False;

   FMouseDownPos := Point (X, Y);

   if not Handled then
       FViewPort.MouseDown(X, Y, Button);
end;

procedure TEditForm.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if assigned (FViewport) then begin
   FViewPort.MouseMove(X, Y, Shift);
   if FViewPort.Updated then
      Render;
  end;
end;

procedure TEditForm.FormMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  OldSelectedFace: TFace;
begin
   FViewPort.MouseUp;

   if (Abs (FMouseDownPos.X - X) < 5) and (Abs (FMouseDownPos.Y - Y) < 4) then
      if (Button = mbLeft) then begin
         OldSelectedFace := FModel.SelectedFace;
         if (Now - FLastMouseUp) > (1 / 24 / 3600 * 0.25) then
             FModel.MouseDown (X, Y, FViewPort, Shift)  // Single Click
         else
             FModel.FaceSelectionToPointSelection;  // Double Click
         if OldSelectedFace <> FModel.SelectedFace then // If Face selection has changed
            ShowSelectedFace;
         UpdateFields;
         UpdateStatus;
         Render;
         FLastMouseUp := Now;
      end;
end;

procedure TEditForm.SpeedButton1Click(Sender: TObject);
begin
   if Sender = SpeedButton1 then FViewPort.SetOrientation(voFront);
   if Sender = SpeedButton4 then FViewPort.SetOrientation(voBack);
   if Sender = SpeedButton5 then FViewPort.SetOrientation(voRight);
   if Sender = SpeedButton2 then FViewPort.SetOrientation(voLeft);
   if Sender = SpeedButton3 then FViewPort.SetOrientation(voTop);
   if Sender = SpeedButton6 then FViewPort.SetOrientation(voBottom);
   if Sender = SpeedButton7 then FViewPort.SetOrientation(voIso);
   Render;
end;

procedure TEditForm.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  SaveSettings;
  Action := caFree;
end;

procedure TEditForm.Background1Click(Sender: TObject);
begin
  if Sender = Background1 then
      ColorDialog.Color := BackgroundColor;
  if Sender = Lines1 then
      ColorDialog.Color := LineColor;
  if Sender = Ground2 then
      ColorDialog.Color := GroundColor;
  if Sender = Points1 then
      ColorDialog.Color := PointColor;

  if ColorDialog.Execute then begin
     if Sender = Background1 then begin
         BackgroundColor := ColorDialog.Color;
         FEngine.SetClearColor(BackgroundColor);
     end;
     if Sender = Lines1 then
         LineColor := ColorDialog.Color;
     if Sender = Ground2 then
         GroundColor := ColorDialog.Color;
     if Sender = Points1 then
         PointColor := ColorDialog.Color;
     Render;
  end;
end;

procedure TEditForm.FormMouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
begin
   FViewPort.MouseWheel(WheelDelta);
   Render;
end;

procedure TEditForm.ViewPoints1Click(Sender: TObject);
begin
  ViewPoints1.Checked := not ViewPoints1.Checked;
  Render_Points := ViewPoints1.Checked;
  Render;
end;

procedure TEditForm.Edges1Click(Sender: TObject);
begin
  Edges1.Checked := not Edges1.Checked;
  Render_Wireframe := Edges1.Checked;
  Render;
end;

procedure TEditForm.Faces1Click(Sender: TObject);
begin
  Faces1.Checked := not Faces1.Checked;
  Render_Faces := Faces1.Checked;
  Render;
end;

procedure TEditForm.Ground1Click(Sender: TObject);
begin
  Ground1.Checked := not Ground1.Checked;
  Render_Ground := Ground1.Checked;
  Render;
end;

procedure TEditForm.Outbox1Click(Sender: TObject);
begin
  Outbox1.Checked := not Outbox1.Checked;
  Render_Outbox := Outbox1.Checked;
  Render;
end;

procedure TEditForm.FormCreate(Sender: TObject);
begin
  Application.UpdateFormatSettings := false;
  Application.UpdateMetricSettings := false;
  ViewPoints1.Checked := Render_Points;
  Edges1.Checked := Render_Wireframe;
  Faces1.Checked := Render_Faces;
  Backfaces1.Checked := Render_Backfaces;
  Ground1.Checked := Render_Ground;
  Outbox1.Checked := Render_Outbox;
  UpdateStatus;
  UpdateFields;
  PageControl.ActivePage := ModelTabSheet;
end;

procedure TEditForm.UpdateFields;
begin
   if not Assigned(FModel) then exit;
   FChangeLock := True;
   if Assigned(FModel.SelectedNode) then begin
      Edit3.Text := FloatToStrF(FModel.SelectedNode.Point[0], fffixed, 9, 2);
      Edit4.Text := FloatToStrF(FModel.SelectedNode.Point[1], fffixed, 9, 2);
      Edit5.Text := FloatToStrF(FModel.SelectedNode.Point[2], fffixed, 9, 2);
      Edit3.Enabled := True;
   end else begin
      Edit3.Enabled := False;
   end;
   Edit4.Enabled := Edit3.Enabled;
   Edit5.Enabled := Edit3.Enabled;
   FChangeLock := False;

end;

procedure TEditForm.Edit3Change(Sender: TObject);
begin
   if FChangeLock or not Assigned (FModel) then exit;
   if not Assigned(FModel.SelectedNode) then exit;

   if (Sender = Edit3) and (Edit3.Text <> '') then
      FModel.SelectedNode.X := StrToFloat(Edit3.Text);
   if (Sender = Edit4) and (Edit4.Text <> '') then
      FModel.SelectedNode.Y := StrToFloat(Edit4.Text);
   if (Sender = Edit5) and (Edit5.Text <> '') then
      FModel.SelectedNode.Z := StrToFloat(Edit5.Text);
   Render;

end;

procedure TEditForm.SpeedButton11Click(Sender: TObject);
var
   Index: Integer;
   Vector: TVector3f;
   Node: TNode;
   Factor: Single;
begin
  For Index := 0 to 2 do
     Vector[Index] := 0;

  Factor := 0.1;

  if Sender = SpeedButton8 then Vector[0] := Factor;
  if Sender = SpeedButton9 then Vector[0] := -Factor;
  if Sender = SpeedButton14 then Vector[1] := -Factor;
  if Sender = SpeedButton15 then Vector[1] := Factor;
  if Sender = SpeedButton10 then Vector[2] := -Factor;
  if Sender = SpeedButton11 then Vector[2] := Factor;

  if Assigned(FModel) then
     For Index := 0 to FModel.NodeCount - 1 do begin
         Node := FModel.Nodes[Index];
         if Node.Selected then
            Node.Point := VectorAdd (Node.Point, Vector)
     end;
  Render;
  UpdateFields;

end;

procedure TEditForm.Button6Click(Sender: TObject);
var
  SelCount: Integer;
begin
  if not Assigned(FModel) then exit;
  SelCount := FModel.SelNodeCount;
  if SelCount = 3 then begin
     FModel.CreateAFace;
     UpdateFields;
     UpdateStatus;
     Render;
  end else begin
     ShowMessage ('You need to select 3 Points for creating a face');
  end;
end;

procedure TEditForm.UpdateStatus;
var
  SelCount: Integer;
  S: String;
begin
   if Assigned(FModel) then begin
      S := Format ('%d Points', [ FModel.NodeCount ]);
      SelCount := FModel.SelNodeCount;
      if SelCount > 0 then
         S := S + Format(' (%d selected)', [ SelCount ]);
      Statusbar.Panels[0].Text := S;

      S := Format ('%d Faces', [ FModel.FaceCount ]);
      SelCount := FModel.SelFaceCount;
      if SelCount > 0 then
         S := S + Format(' (%d selected)', [ SelCount ]);
      Statusbar.Panels[1].Text := S;


   end;
end;

procedure TEditForm.Button10Click(Sender: TObject);
begin
  if Sender = Button10 then
     FModel.SelectAllNodes (True);
  if Sender = Button13 then
     FModel.SelectAllNodes (False);
  if Sender = Button12 then
     FModel.InvertNodeSelection;
  if Sender = Button14 then
     FModel.FaceSelectionToPointSelection;
  if Sender = Button15 then
     FModel.PointSelectionToFaceSelection;
  if Sender = Button8 then
     FModel.SelectAllFaces (True);
  if Sender = Button7 then
     FModel.SelectAllFaces (False);
  if Sender = Button9 then
     FModel.InvertFaceSelection;
  if (Sender = Button2) or (Sender = Button17) then
     FModel.InvertFaces;
  if Sender = Button26 then
     FModel.FloatFaces;
  if Sender = Button4 then
     FModel.DupeNodes;
  if Sender = Button25 then
     FModel.DupeFaces;
  if Sender = Button3 then
     if MessageDlg ('Really delete marked faces?', mtConfirmation, [mbYes, mbNo], 0) = mrYes then
         FModel.RemoveFaces;
  if Sender = Button11 then
     if MessageDlg ('Really delete marked points? Connected faces will be lost!', mtConfirmation, [mbYes, mbNo], 0) = mrYes then
         FModel.RemoveNodes;
  if Sender = Button27 then
     MessageDlg (Format ('%d points removed', [ FModel.RemoveFreeNodes ]), mtInformation, [ mbOK ], 0);


  Render;
  UpdateFields;
  UpdateStatus;
  ShowSelectedFace;
end;

procedure TEditForm.Button1Click(Sender: TObject);
var
  Count: Integer;
begin
  FModel.FixErrors;
  Count := FModel.RemoveInvalidFaces;
  if Count > 0 then
     MessageDlg (Format('removed %d invalid surfaces', [ Count ]), mtInformation, [mbOK], 0);
  UpdateStatus;
  Render;
end;

procedure TEditForm.Button5Click(Sender: TObject);
var
  P: TPoint;
  Form: TCoordForm;
begin
  P := Button5.ClientToScreen (Point(65,20));
  Form := TCoordForm.Create(Self, P.X, P.Y, AddNodeCallback);
  Form.Show;
end;

procedure TEditForm.AddNodeCallback (AX, AY, AZ: Single);
var
  Node: TNode;
begin
     Node := FModel.AddNode(AX, AY, AZ);
     Node.Selected := True;
     Render;
     UpdateStatus;
     UpdateFields;
end;

procedure TEditForm.Backfaces1Click(Sender: TObject);
begin
  Backfaces1.Checked := not Backfaces1.Checked;
  Render_Backfaces := Backfaces1.Checked;
  Render;
end;

procedure TEditForm.Close1Click(Sender: TObject);
begin
  Close;
end;

procedure TEditForm.PreviewPaintBoxPaint(Sender: TObject);
begin
   if Assigned (pArtLoader) and not FPreviewFailed then
     try
        pArtLoader.ShowTile(FTileID + 16384, Point (PreviewPaintBox.Width div 2, PreviewPaintBox.Height div 2), PreviewPaintBox.Canvas);
     except
        FPreviewFailed := True;
     end;
end;

procedure TEditForm.Texturing1Click(Sender: TObject);
begin
  Texturing1.Checked := not Texturing1.Checked;
  Render_Textures := Texturing1.Checked;
  Render;
end;

procedure TEditForm.Button18Click(Sender: TObject);
begin
   MessageDlg (Format ('%d surfaces found', [ FModel.FindSurfaces ]), mtInformation, [mbOk], 0);
end;

procedure TEditForm.Button28Click(Sender: TObject);
begin
   ColorDialog.Color := Shape1.Brush.Color;
   if Assigned(FModel) and ColorDialog.Execute then begin
      FModel.FLightSourceOptions.FR := ColorDialog.Color and 255;
      FModel.FLightSourceOptions.FG := (ColorDialog.Color shr 8) and 255;
      FModel.FLightSourceOptions.FB := (ColorDialog.Color shr 16) and 255;
      Shape1.Brush.Color := ColorDialog.Color;
      Render;
   end;
end;

procedure TEditForm.CheckBox1Click(Sender: TObject);
begin
   if Checkbox1.Checked then
      FModel.Flags := FModel.Flags + [ mfModelIsTileable ]
   else
      FModel.Flags := FModel.Flags - [ mfModelIsTileable ];
end;

procedure TEditForm.CheckBox2Click(Sender: TObject);
begin
   if Assigned (FModel) then
      FModel.FLightSourceOptions.FModelIsLightSource := Checkbox2.Checked;
   Render;
end;

procedure TEditForm.CheckBox3Click(Sender: TObject);
begin
   if Assigned (FModel) then
      FModel.FLightSourceOptions.FFlickering := Checkbox3.Checked;
   Render;
end;

procedure TEditForm.Edit14Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit14.Text <> '') then
      FModel.FLightSourceOptions.FX := StrToFloat(Edit14.Text);
   Render;
end;

procedure TEditForm.Edit15Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit15.Text <> '') then
      FModel.FLightSourceOptions.FY := StrToFloat(Edit15.Text);
   Render;
end;

procedure TEditForm.Edit16Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit16.Text <> '') then
      FModel.FLightSourceOptions.FZ := StrToFloat(Edit16.Text);
   Render;
end;

procedure TEditForm.Edit17Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit17.Text <> '') then begin
      FModel.FLightSourceOptions.FRadius := StrToFloat(Edit17.Text);
      if FModel.FLightSourceOptions.FRadius < 0.5 then
          FModel.FLightSourceOptions.FRadius := 0.5;
   end;
   Render;
end;


procedure TEditForm.Edit18Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit18.Text <> '') then begin
      FModel.FLightSourceOptions.FFlickerMinDelay := StrToInt(Edit18.Text);
      if FModel.FLightSourceOptions.FFlickerMinDelay < 100 then
          FModel.FLightSourceOptions.FFlickerMinDelay := 100;
   end;
   Render;
end;

procedure TEditForm.Edit19Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit19.Text <> '') then begin
      FModel.FLightSourceOptions.FFlickerMaxDelay := StrToInt(Edit19.Text);
      if FModel.FLightSourceOptions.FFlickerMaxDelay < FModel.FLightSourceOptions.FFlickerMinDelay + 10 then
          FModel.FLightSourceOptions.FFlickerMaxDelay := FModel.FLightSourceOptions.FFlickerMinDelay + 10;
   end;
   Render;
end;

procedure TEditForm.Edit20Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit20.Text <> '') then begin
      FModel.FLightSourceOptions.FFlickerAmount := StrToInt(Edit20.Text);
      if FModel.FLightSourceOptions.FFlickerAmount < 1 then
          FModel.FLightSourceOptions.FFlickerAmount := 1;
      if FModel.FLightSourceOptions.FFlickerAmount > 100 then
          FModel.FLightSourceOptions.FFlickerAmount := 100;
   end;
   Render;
end;

procedure TEditForm.PageControlChange(Sender: TObject);
begin
  Render;
  ShowTextures;
end;

procedure TEditForm.Button34Click(Sender: TObject);
var
  Raster, Raster2: TRaster3D;
  ax, ay, az: Integer;
  x, y, z, r: Single;
begin
  r := 0.5;
  Raster2 := FModel.CreateRaster (1 / (r / 16));
  try
    Raster2.FloodFill;
    Raster := Raster2.CreateFuzzyRaster(16, FModel.ShadowIntensity);
  finally
    Raster2.Free;
  end;

  try
    FEngine.MakeCurrent (Canvas.Handle);
    FViewPort.Apply(FEngine, ClientHeight);

    glClear(GL_DEPTH_BUFFER_BIT or GL_COLOR_BUFFER_BIT);
    glEnable (GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glColor4f(1.0,1.0,1.0,1.0);
    glEnable (GL_LIGHTING);

    FViewPort.LoadView;
    glTranslatef (-0.5, -0.5, -FModel.Height / 20);
    For aX := 0 to Raster.SizeX - 1 do
      For aY := 0 to Raster.SizeY - 1 do
        For aZ := 0 to Raster.SizeZ - 1 do
            if Raster[aX, aY, aZ] > 0 then begin
                SetLightColor (Raster[aX, aY, aZ] * $010101);
                X := (aX + Raster.MinX)* r;
                Y := (aY + Raster.MinY) * r;
                Z := (aZ + Raster.MinZ) * r;
                glBegin(GL_QUADS);
                glNormal3f(0,0,-1);
                glVertex3f(X, Y, Z);
                glVertex3f(X+r, Y, Z);
                glVertex3f(X+r, Y+r, Z);
                glVertex3f(X, Y+r, Z);
                glNormal3f(0,0,1);
                glVertex3f(X, Y, Z+r);
                glVertex3f(X+r, Y, Z+r);
                glVertex3f(X+r, Y+r, Z+r);
                glVertex3f(X, Y+r, Z+r);
                glNormal3f(-1,0,0);
                glVertex3f(X, Y, Z);
                glVertex3f(X, Y, Z+r);
                glVertex3f(X, Y+r, Z+r);
                glVertex3f(X, Y+r, Z);
                glNormal3f(1,0,0);
                glVertex3f(X+r, Y, Z);
                glVertex3f(X+r, Y, Z+r);
                glVertex3f(X+r, Y+r, Z+r);
                glVertex3f(X+r, Y+r, Z);
                glNormal3f(0,-1,0);
                glVertex3f(X, Y, Z);
                glVertex3f(X+r, Y, Z);
                glVertex3f(X+r, Y, Z+r);
                glVertex3f(X, Y, Z+r);
                glNormal3f(0,1,0);
                glVertex3f(X, Y+r, Z);
                glVertex3f(X+r, Y+r, Z);
                glVertex3f(X+r, Y+r, Z+r);
                glVertex3f(X, Y+r, Z+r);
                glEnd();
            end;
    SwapBuffers (Canvas.Handle);
  finally
      Raster.Free;
  end;

end;

procedure TEditForm.SpeedButton29Click(Sender: TObject);
var
   Index: Integer;
   Vector: TVector3f;
   Factor: Single;
begin
  if not Assigned (FModel) then exit;
  For Index := 0 to 2 do
     Vector[Index] := 0;

  Factor := 0.1;

  if Sender = SpeedButton31 then Vector[0] := Factor;
  if Sender = SpeedButton29 then Vector[0] := -Factor;
  if Sender = SpeedButton34 then Vector[1] := -Factor;
  if Sender = SpeedButton32 then Vector[1] := Factor;
  if Sender = SpeedButton30 then Vector[2] := -Factor;
  if Sender = SpeedButton33 then Vector[2] := Factor;

  FModel.FLightSourceOptions.FX := FModel.FLightSourceOptions.FX - Vector[0];
  FModel.FLightSourceOptions.FY := FModel.FLightSourceOptions.FY - Vector[1];
  FModel.FLightSourceOptions.FZ := FModel.FLightSourceOptions.FZ - Vector[2];

  Edit14.Text := Format('%.2f', [ FModel.FLightSourceOptions.FX ]);
  Edit15.Text := Format('%.2f', [ FModel.FLightSourceOptions.FY ]);
  Edit16.Text := Format('%.2f', [ FModel.FLightSourceOptions.FZ ]);
end;

procedure TEditForm.TrackBar1Change(Sender: TObject);
begin
   if Assigned (FModel) then
     FModel.ShadowIntensity := Trackbar1.Position;
end;

procedure TEditForm.ShowSelectedFace;
var
  Index: Integer;
begin
  Button2.Enabled := FModel.SelFaceCount > 0;
  Button3.Enabled := Button2.Enabled;
  Button25.Enabled := Button2.Enabled;
  Button26.Enabled := Button2.Enabled;
  Label6.Enabled := Assigned (FModel.SelectedFace);
  Edit13.Enabled := Assigned (FModel.SelectedFace);
  For Index := 0 to Groupbox1.ControlCount - 1 do
      Groupbox1.Controls[Index].Enabled := Assigned (FModel.SelectedFace);
  if Assigned (FModel.SelectedFace) then begin
      Edit13.Text := IntToStr(FModel.SelectedFace.LightSurfaceIndex);
      Edit7.Text := FloatToStrF(FModel.SelectedFace.TexCoords[0, 0], fffixed, 5, 2);
      Edit10.Text := FloatToStrF(FModel.SelectedFace.TexCoords[0, 1], fffixed, 5, 2);
      Edit8.Text := FloatToStrF(FModel.SelectedFace.TexCoords[1, 0], fffixed, 5, 2);
      Edit11.Text := FloatToStrF(FModel.SelectedFace.TexCoords[1, 1], fffixed, 5, 2);
      Edit9.Text := FloatToStrF(FModel.SelectedFace.TexCoords[2, 0], fffixed, 5, 2);
      Edit12.Text := FloatToStrF(FModel.SelectedFace.TexCoords[2, 1], fffixed, 5, 2);
  end;
end;

procedure TEditForm.Edit7Change(Sender: TObject);
begin
   if Assigned (FModel.SelectedFace) then begin
       if (Sender = Edit7) and ((Sender as TEdit).Text <> '') then
          FModel.SelectedFace.TexCoords[0, 0] := StrToFloat((Sender as TEdit).Text);
       if (Sender = Edit10) and ((Sender as TEdit).Text <> '') then
          FModel.SelectedFace.TexCoords[0, 1] := StrToFloat((Sender as TEdit).Text);
       if (Sender = Edit8) and ((Sender as TEdit).Text <> '') then
          FModel.SelectedFace.TexCoords[1, 0] := StrToFloat((Sender as TEdit).Text);
       if (Sender = Edit11) and ((Sender as TEdit).Text <> '') then
          FModel.SelectedFace.TexCoords[1, 1] := StrToFloat((Sender as TEdit).Text);
       if (Sender = Edit9) and ((Sender as TEdit).Text <> '') then
          FModel.SelectedFace.TexCoords[2, 0] := StrToFloat((Sender as TEdit).Text);
       if (Sender = Edit12) and ((Sender as TEdit).Text <> '') then
          FModel.SelectedFace.TexCoords[2, 1] := StrToFloat((Sender as TEdit).Text);
       if (Sender = Edit13) and ((Sender as TEdit).Text <> '') then
          FModel.SelectedFace.SurfaceIndex := StrToInt((Sender as TEdit).Text);
       Render;

   end;

end;

procedure TEditForm.SpeedButton18Click(Sender: TObject);
var
  Dir: Integer;
  Edit: TEdit;
begin
   Edit := nil; Dir := 0;
   if Sender = SpeedButton18 then begin Dir := 1; Edit := Edit10; end;
   if Sender = SpeedButton19 then begin Dir := -1; Edit := Edit10; end;
   if Sender = SpeedButton16 then begin Dir := -1; Edit := Edit7; end;
   if Sender = SpeedButton17 then begin Dir := 1; Edit := Edit7; end;
   if Sender = SpeedButton21 then begin Dir := 1; Edit := Edit11; end;
   if Sender = SpeedButton22 then begin Dir := -1; Edit := Edit11; end;
   if Sender = SpeedButton20 then begin Dir := -1; Edit := Edit8; end;
   if Sender = SpeedButton23 then begin Dir := 1; Edit := Edit8; end;
   if Sender = SpeedButton26 then begin Dir := 1; Edit := Edit12; end;
   if Sender = SpeedButton24 then begin Dir := -1; Edit := Edit12; end;
   if Sender = SpeedButton27 then begin Dir := -1; Edit := Edit9; end;
   if Sender = SpeedButton25 then begin Dir := 1; Edit := Edit9; end;
   if Assigned (Edit) then
       if Edit.Text <> '' then
          Edit.Text := FloatToStrF(StrToFloat(Edit.Text) + Dir * 0.1, fffixed, 5, 2);
end;

procedure TEditForm.Button29Click(Sender: TObject);
begin
  FModel.SetSmoothLighted;
  ShowSelectedFace;
end;

procedure TEditForm.Button30Click(Sender: TObject);
begin
  FModel.SetEdgeLighted;
  ShowSelectedFace;
end;

procedure TEditForm.CheckBox4Click(Sender: TObject);
begin
   if Checkbox4.Checked then
      FModel.Flags := FModel.Flags + [ mfModelCastsShadows ]
   else
      FModel.Flags := FModel.Flags - [ mfModelCastsShadows ];
end;

procedure TEditForm.Button19Click(Sender: TObject);
begin
    if (Sender = Button19) or (Sender = X1) then FModel.MirrorX;
    if (Sender = Button20) or (Sender = Y1) then FModel.MirrorY;
    if (Sender = Button21) or (Sender = Z1) then FModel.MirrorZ;
    if (Sender = Button22) or (Sender = N901) then FModel.Rotate90;
    if (Sender = Button23) or (Sender = N1801) then FModel.Rotate180;
    if (Sender = Button24) or (Sender = N2701) then FModel.Rotate270;
    ShowSelectedFace;
    Render;
end;

procedure TEditForm.Button35Click(Sender: TObject);
var
  Form: TScaleForm;
  X, Y, Z: Single;
begin
  Form := TScaleForm.Create(Self);
  try
      if Form.ShowModal = mrOK then begin
          X := StrToFloat(Form.Edit1.Text);
          Y := StrToFloat(Form.Edit2.Text);
          Z := StrToFloat(Form.Edit3.Text);
          if (X = 0) or (Y = 0) or (Z = 0) then
              raise Exception.Create ('Can not scale by 0..');
          FModel.Scale(AffineVectorMake(X, Y, Z));
          Render;
      end;
  finally
    Form.Free;
  end;

end;

procedure TEditForm.ranslate1Click(Sender: TObject);
begin
  //
end;

procedure TEditForm.Button37Click(Sender: TObject);
var
  AName: String;
  result: TJPEGTexture;

begin
   if OpenPictureDialog.Execute then begin
      AName := ExtractFileName(OpenPictureDialog.FileName);
      if InputQuery ('Add Texture..', 'Please enter a name:', AName) then begin
         result := pTextureManager.AddTexture (AName, OpenPictureDialog.FileName);
         if result.Name <> AName then
            MessageDlg (Format ('Warning: Texture "%s" existed: "%s"', [ AName, result.Name ]), mtWarning, [mbOk], 0);
         FUsedTextures.putPair([result, 0]);
         ShowTextures;
      end;
   end;
end;

procedure TEditForm.ShowTextures;
var
  Iter: DIterator;
  ATexture: TTexture;
  Item: TListItem;
  ArtTexture: TArtTexture;
  ArtCount: Integer;
begin
  ListView1.Items.BeginUpdate;
  ListView1.Items.Clear;

  ArtTexture := nil; ArtCount := 0;

  FModel.CalcFaceTextureCount (FTextureList); // Calc "in use list"
  Iter := FTextureList.InUseCounter.start;  // Update current texture use list, but don't
                                            // delete unused textures
  while not atEnd (Iter) do begin
      SetToKey (Iter);
      ATexture := getObject (Iter) as TTexture;
      SetToValue (Iter);
      FUsedTextures.putPair([ATexture, getInteger(Iter)]);
      Advance (Iter);
  end;

  Iter := FUsedTextures.start;  // Set usecount of removed textures to 0
  while not atEnd (Iter) do begin
      SetToKey (Iter);
      ATexture := getObject (Iter) as TTexture;
      if atEnd (FTextureList.InUseCounter.locate([ATexture])) then
          FUsedTextures.putPair([ATexture, 0]);
      Advance (Iter);
  end;

  Iter := FUsedTextures.start;
  while not atEnd (Iter) do begin
      SetToKey (Iter);
      ATexture := getObject (Iter) as TTexture;
      SetToValue (Iter);
      if ATexture is TJPEGTexture then begin  // if JPEG texture then show in List
          Item := ListView1.Items.Add;
          Item.Caption := TJPEGTexture(ATexture).Name;
          Item.SubItems.Add(IntToStr(getInteger(Iter)));
          Item.Data := ATexture;
      end;
      if ATexture is TArtTexture then begin // if Art Texture then save it for later
          ArtTexture := TArtTexture (ATexture);
          ArtCount := getInteger(Iter);
      end;
      Advance (Iter);
  end;

  if Assigned (ArtTexture) then begin
       Item := ListView1.Items.Add;
       Item.Caption := 'UO Texture';
       Item.SubItems.Add(IntToStr(ArtCount));
       Item.Data := ArtTexture;
  end;

  ListView1.Items.EndUpdate;
end;

procedure TEditForm.Button33Click(Sender: TObject);
begin
  FUsedTextures.clear;
  ShowTextures;
end;

procedure TEditForm.Button32Click(Sender: TObject);
begin
  FModel.SelectFacesByTexture(nil);
  ShowSelectedFace;
  UpdateFields;
  UpdateStatus;
  Render;
end;

procedure TEditForm.SelectFaces1Click(Sender: TObject);
begin
  if Assigned (ListView1.Selected) then begin
      FModel.SelectFacesByTexture(ListView1.Selected.Data);
      ShowSelectedFace;
      UpdateFields;
      UpdateStatus;
      Render;
  end;
end;

procedure TEditForm.ApplyTexture1Click(Sender: TObject);
begin
  if Assigned (ListView1.Selected) then begin
      FModel.ApplyTexture(ListView1.Selected.Data, True);
      Render;
      ShowTextures;
  end;
end;

procedure TEditForm.Button38Click(Sender: TObject);
begin
      FModel.ApplyTexture(pTextureManager.AddArtTexture (FTileID), True);
      Render;
      ShowTextures;
end;

procedure TEditForm.Button31Click(Sender: TObject);
begin
     if TextureManagerForm.ShowModal = mrOK then begin
       if Assigned (TextureManagerForm.ResultTexture) then
           FUsedTextures.putPair([TextureManagerForm.ResultTexture, 0]);
       ShowTextures;
     end;
end;

procedure TEditForm.selectallfaces1Click(Sender: TObject);
begin
      FModel.SelectAllFaces(Sender = selectallfaces1);
      ShowSelectedFace;
      UpdateFields;
      UpdateStatus;
      Render;
end;

procedure TEditForm.Edit6Click(Sender: TObject);
begin
  Paste1.Enabled := Assigned (pModelClipboard);
  PasteLightOptions1.Enabled := not pLightClipboardEmpty;
end;

procedure TEditForm.Copy1Click(Sender: TObject);
begin
   pModelClipboard.Free;
   pModelClipboard := nil;
   pModelClipboard := TModel.CreateFromModel(FModel, 0);
end;

procedure TEditForm.CopyLightOptions1Click(Sender: TObject);
begin
   pLightClipboardEmpty := False;
   pLightClipboard := FModel.FLightSourceOptions;
end;

procedure TEditForm.PasteLightOptions1Click(Sender: TObject);
begin
   if not pLightClipboardEmpty then
     if MessageDlg ('Are you sure to overwrite the current light settings?', mtConfirmation, [mbYes, mbNo], 0) = mrYes then begin
         FModel.FLightSourceOptions := pLightClipboard;
         ShowLightSettings;
     end;
end;

procedure TEditForm.ShowLightSettings;
begin
   Checkbox1.Checked := mfModelIsTileable in FModel.Flags;
   Checkbox4.Checked := mfModelCastsShadows in FModel.Flags;

   Shape1.Brush.Color := FModel.FLightSourceOptions.FR + FModel.FLightSourceOptions.FG shl 8 + FModel.FLightSourceOptions.FB shl 16;
   Checkbox2.Checked := FModel.FLightSourceOptions.FModelIsLightSource;
   Checkbox3.Checked := FModel.FLightSourceOptions.FFlickering;
   Edit14.Text := Format('%.2f', [ FModel.FLightSourceOptions.FX ]);
   Edit15.Text := Format('%.2f', [ FModel.FLightSourceOptions.FY ]);
   Edit16.Text := Format('%.2f', [ FModel.FLightSourceOptions.FZ ]);
   Edit17.Text := Format('%.2f', [ FModel.FLightSourceOptions.FRadius ]);
   Edit18.Text := IntToStr( FModel.FLightSourceOptions.FFlickerMinDelay );
   Edit19.Text := IntToStr( FModel.FLightSourceOptions.FFlickerMaxDelay );
   Edit20.Text := IntToStr( FModel.FLightSourceOptions.FFlickerAmount );
   Trackbar1.Position := FModel.ShadowIntensity;
end;

procedure TEditForm.ShowEffectSettings;
begin
   Checkbox6.Checked := mfModelHasParticleEffect in FModel.Flags;
   Checkbox7.Checked := FModel.FEffectOptions.FRotateEffect;

   Edit21.Text := Format('%.2f', [ FModel.FEffectOptions.FX ]);
   Edit22.Text := Format('%.2f', [ FModel.FEffectOptions.FY ]);
   Edit23.Text := Format('%.2f', [ FModel.FEffectOptions.FZ ]);
   Edit25.Text := Format('%.2f', [ FModel.FEffectOptions.FAngle1 ]);
   Edit26.Text := Format('%.2f', [ FModel.FEffectOptions.FAngle2 ]);
   Trackbar1.Position := FModel.ShadowIntensity;

   Edit24.Text := FModel.ParticleEffectName;
end;


procedure TEditForm.Paste1Click(Sender: TObject);
begin
   if Assigned (pModelClipboard) then
       if MessageDlg ('Are you sure to paste the clipboard?', mtConfirmation, [mbYes, mbNo], 0) = mrYes then begin
           FModel.AssignFromModel(pModelClipboard);
           Render;
           ShowSelectedFace;
           Render;
           ShowTextures;
       end;

end;

procedure TEditForm.Import3DS1Click(Sender: TObject);
var
  AName: String;
  ATexture: TJPEGTexture;
  AModel: TModel;
begin
       if OpenDialog3DS.Execute then begin
            AModel := TModel.CreateFrom3DS(0, OpenDialog3DS.FileName);
            try
                if OpenPictureDialog.Execute then begin
                    AName := ExtractFileName(OpenPictureDialog.FileName);
                    if InputQuery ('Add Texture..', 'Please enter a name:', AName) then begin
                         ATexture := pTextureManager.AddTexture (AName, OpenPictureDialog.FileName);
                         if ATexture.Name <> AName then
                              MessageDlg (Format ('Warning: Texture "%s" existed: "%s"', [ AName, ATexture.Name ]), mtWarning, [mbOk], 0);
                         AModel.ApplyTexture(ATexture, False);
                    end;
                end;
                FModel.AssignFromModel(AModel);
            finally
                AModel.Free;
            end;
       end;
end;

procedure TEditForm.CheckBox6Click(Sender: TObject);
begin
   if Checkbox6.Checked then
      FModel.Flags := FModel.Flags + [ mfModelHasParticleEffect ]
   else
      FModel.Flags := FModel.Flags - [ mfModelHasParticleEffect ];
   Render;
end;

procedure TEditForm.CheckBox7Click(Sender: TObject);
begin
  FModel.FEffectOptions.FRotateEffect := Checkbox7.Checked;
  Render;
end;

procedure TEditForm.Edit21Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit21.Text <> '') then
      FModel.FEffectOptions.FX := StrToFloat(Edit21.Text);
   Render;
end;

procedure TEditForm.Edit22Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit22.Text <> '') then
      FModel.FEffectOptions.FY := StrToFloat(Edit22.Text);
   Render;
end;

procedure TEditForm.Edit23Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit23.Text <> '') then
      FModel.FEffectOptions.FZ := StrToFloat(Edit23.Text);
   Render;
end;

procedure TEditForm.Edit25Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit25.Text <> '') then
      FModel.FEffectOptions.FAngle1 := StrToFloat(Edit25.Text);
   Render;
end;

procedure TEditForm.Edit26Change(Sender: TObject);
begin
   if Assigned (FModel) and (Edit26.Text <> '') then
      FModel.FEffectOptions.FAngle2 := StrToFloat(Edit26.Text);
   Render;
end;

procedure TEditForm.Edit24Change(Sender: TObject);
begin
   FModel.ParticleEffectName := Edit24.Text;
end;

procedure TEditForm.SpeedButton12Click(Sender: TObject);
var
   Index: Integer;
   Vector: TVector3f;
   Factor: Single;
begin
  if not Assigned (FModel) then exit;
  For Index := 0 to 2 do
     Vector[Index] := 0;

  Factor := 0.1;

  if Sender = SpeedButton37 then Vector[1] := Factor;
  if Sender = SpeedButton12 then Vector[1] := -Factor;
  if Sender = SpeedButton40 then Vector[0] := -Factor;
  if Sender = SpeedButton38 then Vector[0] := Factor;
  if Sender = SpeedButton39 then Vector[2] := Factor;
  if Sender = SpeedButton36 then Vector[2] := -Factor;

  FModel.FEffectOptions.FX := FModel.FEffectOptions.FX - Vector[0];
  FModel.FEffectOptions.FY := FModel.FEffectOptions.FY - Vector[1];
  FModel.FEffectOptions.FZ := FModel.FEffectOptions.FZ - Vector[2];

  Edit21.Text := Format('%.2f', [ FModel.FEffectOptions.FX ]);
  Edit22.Text := Format('%.2f', [ FModel.FEffectOptions.FY ]);
  Edit23.Text := Format('%.2f', [ FModel.FEffectOptions.FZ ]);
end;

end.
