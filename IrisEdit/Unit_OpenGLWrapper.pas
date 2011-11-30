{==============================================================================

  Iris Model Editor - Copyright by Alexander Oster, tensor@ultima-iris.de

 The contents of this file are used with permission, subject to
 the Mozilla Public License Version 1.1 (the "License"); you may
 not use this file except in compliance with the License. You may
 obtain a copy of the License at
 http://www.mozilla.org/MPL/MPL-1.1.html

 Software distributed under the License is distributed on an
 "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 implied. See the License for the specific language governing
 rights and limitations under the License.

==============================================================================}

unit Unit_OpenGLWrapper;

interface

uses Windows,
  Unit_OpenGL,
  Unit_OpenGLGeometry,
  SysUtils,
  Graphics;

const
  FarClipping       = 10000.0;
  NearClipping      = -10000.0;

type
  TGLEngine = class(TObject)
  PROTECTED
    FInitialized: Boolean;
    FHandle: HDC;
    FRC: HGLRC;
    FXPos, FYPos: Integer;
    FSize: Single;
    FRatio: Single;
    FWidth, FHeight: Integer;
    FMustbeInitialized: Boolean;
    procedure SetPixelFormat(Handle: HDC);
  PUBLIC
    property Initialized: Boolean READ FInitialized;
    property Ratio: Single READ FRatio WRITE FRatio;
    property MustbeInitialized: Boolean READ FMustbeInitialized WRITE
      FMustbeInitialized;
    constructor Create;
    destructor Destroy; OVERRIDE;
    function StartGL(Handle: HDC): Boolean;
    procedure SetupGL;
    procedure DeInit (Handle: HDC);
    procedure SetViewPortOrtho(XPos, YPos, Width, Height: Integer; Size: Single
      =
      -1);
    procedure SetViewPortPerspective(XPos, YPos, Width, Height: Integer);
    procedure ResetMaterial;
    procedure MakeCurrent(Handle: HDC);
    procedure SwapBuffers(Handle: HDC);
  end;

procedure SetClearColor(Color: TColor);
procedure SetColor(AColor: TColor; Transparency: Single = 1.0);
procedure SetMaterial(AAmbient, ADiffuse, ASpecular, AShininess: Single);
procedure SetColorMaterial(Specular, Ambient, Diffuse: TColor; Shininess:
  Single);
procedure SetLightColor (Color: TColor);

const
  light_position    : array[0..3] of TGLfloat = (-2.0, 2.0, 2.0, 0.0);
  light_ambient     : array[0..3] of TGLfloat = (0.40, 0.40, 0.40, 1.0);
  mat_specular      : array[0..3] of TGLfloat = (0.2, 0.2, 0.2, 1.0);
  mat_shininess     : array[0..0] of TGLfloat = (0.2);
  mat_ambient       : array[0..3] of TGLfloat = (0.9, 0.9, 0.9, 1.0);
  mat_diffuse       : array[0..3] of TGLfloat = (0.7, 0.7, 0.7, 1.0);
  mat_green         : array[0..3] of TGLfloat = (0.0, 0.8, 0.0, 1.0);
  mat_yellow        : array[0..3] of TGLfloat = (0.0, 0.8, 0.8, 1.0);
  mat_red           : array[0..3] of TGLfloat = (0.8, 0.0, 0.0, 1.0);

var
    OpenGLIsInitialized: Boolean = True;

implementation



constructor TGLEngine.Create;
begin
  FInitialized := False;
  FRatio := 1;
  FRC := 0;
  FSize := 200;
  FMustbeInitialized := True;
end;

destructor TGLEngine.Destroy;
begin
  if FInitialized and FMustbeInitialized then
    Deinit (FHandle);
end;

function TGLEngine.StartGL(Handle: HDC): Boolean;
begin
  result := True;
  if not OpenGLIsInitialized then begin
      InitOpenGL;
      OpenGLIsInitialized := True;
  end;

   if FInitialized then exit;
  { if not LoadOpenGL then exit;
}
   SetPixelFormat(Handle);

{   FRC := wglCreateContext(Handle);
   if (FRC=0) then
     raise Exception.Create('could not create RC');

   if (not wglMakeCurrent(Handle, FRC)) then
     raise Exception.Create('OpenGL Error');

{   ClearExtensions;
   ReadExtensions;}

  // OpenGL-Funtionen initialisieren
//     Renderkontext erstellen (32 Bit Farbtiefe, 24 Bit Tiefenpuffer, Doublebuffering)
    FRC := CreateRenderingContext(Handle, [opDoubleBuffered], 32, 16, 0, 0, 0, 0);

  // Erstellten Renderkontext aktivieren
  ActivateRenderingContext(Handle, FRC);

  FInitialized := True;
  FHandle := Handle;
end;

procedure TGLEngine.SetupGL;
begin
  if (not FInitialized and FMustbeInitialized) then
    exit;

  glClearColor(0.49, 0.52, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(gl_Cull_Face);

  glClearDepth(1.0);                    // Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);                 // The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);              // Enables Depth Testing
  glShadeModel(GL_FLAT);                // Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_NORMALIZE);

  glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_specular[0]);
  glMaterialfv(GL_FRONT, GL_SHININESS, @mat_shininess[0]);
  glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_ambient[0]);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_diffuse[0]);

  glLightfv(GL_LIGHT0, GL_POSITION, @light_position[0]);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, @light_ambient[0]);
  glMatrixMode(GL_MODELVIEW);

end;

procedure TGLEngine.DeInit;
begin
  if FRC <> 0 then
  begin
    if (not wglMakeCurrent(FHandle, 0)) then
      MessageBox(0, 'Release of DC and RC failed.', ' Shutdown Error', MB_OK or
        MB_ICONERROR);
    if (not wglDeleteContext(FRC)) then
    begin
      MessageBox(0, 'Release of Rendering Context failed.', ' Shutdown Error',
        MB_OK or MB_ICONERROR);
    end
  end;
  FInitialized := False;
end;

procedure TGLEngine.SetViewPortOrtho(XPos, YPos, Width, Height: Integer; Size:
  Single);
var
  ASizeY            : Integer;
begin
  glViewport(XPos, YPos, Width, Height);
  FXPos := XPos;
  FYPos := YPos;
  FWidth := Width;
  FHeight := Height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if Size > 0 then
    FSize := Size;
  ASizeY := trunc(FSize * Ratio);
  glOrtho(-FSize / 2, FSize / 2, -ASizeY / 2, ASizeY / 2, NearClipping,
    FarClipping);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity;
end;

procedure TGLEngine.SetViewPortPerspective(XPos, YPos, Width, Height: Integer);
//var
//  ASizeY: Integer;
begin
  if Height <= 0 then
    Height := 1;
  glViewport(XPos, YPos, Width, Height);
  FXPos := XPos;
  FYPos := YPos;
  FWidth := Width;
  FHeight := Height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, Width / Height, 1, FarClipping);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity;
end;

procedure TGLEngine.SetPixelFormat(Handle: HDC);
var
  PixelFormat       : TGLuint;
  PFD               : pixelformatdescriptor;
begin
  Fillchar(PFD, sizeof(PFD), 0);
  with pfd do
  begin
    nSize := SizeOf(PIXELFORMATDESCRIPTOR);
    nVersion := 1;
    dwFlags := PFD_DRAW_TO_WINDOW
      or PFD_SUPPORT_OPENGL
      or PFD_DOUBLEBUFFER;
    iPixelType := PFD_TYPE_RGBA;
    cColorBits := 32;
    cRedBits := 0;
    cRedShift := 0;
    cGreenBits := 0;
    cBlueBits := 0;
    cBlueShift := 0;
    cAlphaBits := 0;
    cAlphaShift := 0;
    cAccumBits := 0;
    cAccumRedBits := 0;
    cAccumGreenBits := 0;
    cAccumBlueBits := 0;
    cAccumAlphaBits := 0;
    cDepthBits := 16;
    cStencilBits := 0;
    cAuxBuffers := 0;
    iLayerType := PFD_MAIN_PLANE;
    bReserved := 0;
    dwLayerMask := 0;
    dwVisibleMask := 0;
    dwDamageMask := 0
  end;

  if Handle = 0 then
    raise Exception.Create('Invalid OpenGL - Handle!');

  PixelFormat := ChoosePixelFormat(Handle, @pfd);
  if (PixelFormat = 0) then
    raise
      Exception.Create('Could not choose Pixel format' + IntToStr(GetLastError()));
  if (not Windows.SetPixelFormat(Handle, PixelFormat, @pfd)) then
    raise
      Exception.Create('Could not set Pixel format' + IntToStr(GetLastError()));
end;                                    (*SetPixelFormat*)

procedure TGLEngine.ResetMaterial;
begin
  glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_specular[0]);
  glMaterialfv(GL_FRONT, GL_SHININESS, @mat_shininess[0]);
  glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_ambient[0]);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_diffuse[0]);
end;

procedure TGLEngine.MakeCurrent(Handle: HDC);
begin
    FHandle := Handle;
    wglMakeCurrent(Handle, FRC);
end;

procedure TGLEngine.SwapBuffers(Handle: HDC);
begin
  MakeCurrent(Handle);
  Windows.SwapBuffers(Handle);
end;

procedure SetClearColor(Color: TColor);
begin
  glClearColor((Color and $FF) / 255, ((Color shr 8) and $FF) / 255, ((Color shr
    16) and $FF) / 255, 1.0);
end;

procedure SetColor(AColor: TColor; Transparency: Single = 1.0);
begin
  glColor4f((AColor and $FF) / 255, ((AColor shr 8) and $FF) / 255, ((AColor shr
    16) and $FF) / 255, Transparency);
end;

procedure SetMaterial(AAmbient, ADiffuse, ASpecular, AShininess: Single);
var
  mat_specular      : array[0..3] of TGLfloat;
  mat_shininess     : array[0..0] of TGLfloat;
  mat_ambient       : array[0..3] of TGLfloat;
  mat_diffuse       : array[0..3] of TGLfloat;
begin

  mat_specular[0] := ASpecular;
  mat_specular[1] := ASpecular;
  mat_specular[2] := ASpecular;
  mat_specular[3] := 1.0;

  mat_ambient[0] := AAmbient;
  mat_ambient[1] := AAmbient;
  mat_ambient[2] := AAmbient;
  mat_ambient[3] := 1.0;

  mat_diffuse[0] := ADiffuse;
  mat_diffuse[1] := ADiffuse;
  mat_diffuse[2] := ADiffuse;
  mat_diffuse[3] := 1.0;

  mat_shininess[0] := AShininess * 255;

  glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_specular[0]);
  glMaterialfv(GL_FRONT, GL_SHININESS, @mat_shininess[0]);
  glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_ambient[0]);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_diffuse[0]);
end;

procedure SetColorMaterial(Specular, Ambient, Diffuse: TColor; Shininess:
  Single);
var
  mat_specular      : array[0..3] of TGLfloat;
  mat_shininess     : array[0..0] of TGLfloat;
  mat_ambient       : array[0..3] of TGLfloat;
  mat_diffuse       : array[0..3] of TGLfloat;
begin

  mat_specular[0] := ((Specular shr 0) and $FF) / 255;
  mat_specular[1] := ((Specular shr 8) and $FF) / 255;
  mat_specular[2] := ((Specular shr 16) and $FF) / 255;
  mat_specular[3] := 1.0;

  mat_ambient[0] := ((Ambient shr 0) and $FF) / 255;
  mat_ambient[1] := ((Ambient shr 8) and $FF) / 255;
  mat_ambient[2] := ((Ambient shr 16) and $FF) / 255;
  mat_ambient[3] := 1.0;

  mat_diffuse[0] := ((Diffuse shr 0) and $FF) / 255;
  mat_diffuse[1] := ((Diffuse shr 8) and $FF) / 255;
  mat_diffuse[2] := ((Diffuse shr 16) and $FF) / 255;
  mat_diffuse[3] := 1.0;

  mat_shininess[0] := Shininess;

  glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_specular[0]);
  glMaterialfv(GL_FRONT, GL_SHININESS, @mat_shininess[0]);
  glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_ambient[0]);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_diffuse[0]);
end;

    procedure SetLightColor (Color: TColor);
    var
       facs:  array[0..3] of TGLfloat ;
       mat: array[0..3] of TGLfloat;
       Index: Integer;
    begin
     facs[0] := (Color and $FF) / 255;
     facs[1] := ((Color shr 8) and $FF) / 255;
     facs[2] := ((Color shr 16) and $FF) / 255;
     facs[3] := 1.0;

     for Index := 0 to 3 do
         mat[Index] := mat_ambient[Index] * facs[Index];
     glMaterialfv(GL_FRONT, GL_AMBIENT, @mat[0]);
     for Index := 0 to 3 do
         mat[Index] := mat_diffuse[Index] * facs[Index];
     glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat[0]);
    end;

initialization
      InitOpenGL;

end.
