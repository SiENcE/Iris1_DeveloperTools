program IrisConfig;

uses
  Forms,
  Main in 'Main.pas' {MainForm},
  libxmlparser in 'SDL\libxmlparser.pas',
  logger in 'SDL\logger.pas',
  moduleloader in 'SDL\moduleloader.pas',
  registryuserpreferences in 'SDL\registryuserpreferences.pas',
  sdl in 'SDL\sdl.pas',
  sdl_cpuinfo in 'SDL\sdl_cpuinfo.pas',
  sdlgameinterface in 'SDL\sdlgameinterface.pas',
  sdli386utils in 'SDL\sdli386utils.pas',
  sdlinput in 'SDL\sdlinput.pas',
  sdlstreams in 'SDL\sdlstreams.pas',
  sdlticks in 'SDL\sdlticks.pas',
  sdlutils in 'SDL\sdlutils.pas',
  sdlwindow in 'SDL\sdlwindow.pas',
  userpreferences in 'SDL\userpreferences.pas',
  Unit_XmlNode in 'Unit_XmlNode.pas',
  LibXmlComps in 'LibXmlComps.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'Iris Configuration Tool';
  Application.CreateForm(TMainForm, MainForm);
  Application.Run;
end.
