program Project1;

uses
  Forms,
  Unit1 in 'Unit1.pas' {MainForm},
  Unit_OpenGLGeometry in 'Unit_OpenGLGeometry.pas',
  Unit_OpenGL in 'Unit_OpenGL.pas',
  Unit_OpenGLWrapper in 'Unit_OpenGLWrapper.pas',
  Unit_ViewPort in 'Unit_ViewPort.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TMainForm, MainForm);
  Application.Run;
end.
