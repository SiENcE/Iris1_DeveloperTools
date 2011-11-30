program IrisMapEditor;

uses
  Forms,
  Main in 'Main.pas' {MainForm},
  Form_Heightmap in 'Form_Heightmap.pas' {EditForm},
  Unit_Map in 'Unit_Map.pas',
  Unit_Matrix in 'Unit_Matrix.pas',
  Unit_OpenGL in 'Unit_OpenGL.pas',
  Unit_OpenGLGeometry in 'Unit_OpenGLGeometry.pas',
  Unit_ViewPort in 'Unit_ViewPort.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TMainForm, MainForm);
  Application.Run;
end.
