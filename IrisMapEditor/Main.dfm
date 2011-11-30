object MainForm: TMainForm
  Left = 87
  Top = 125
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Iris Map Editor  0.31'
  ClientHeight = 545
  ClientWidth = 714
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 512
    Width = 47
    Height = 13
    Caption = 'Selection:'
  end
  object Label2: TLabel
    Left = 144
    Top = 512
    Width = 9
    Height = 13
    Caption = 'to'
  end
  object Label4: TLabel
    Left = 400
    Top = 512
    Width = 30
    Height = 13
    Caption = 'Detail:'
  end
  object Label5: TLabel
    Left = 16
    Top = 8
    Width = 43
    Height = 13
    Caption = 'Map File:'
  end
  object Label6: TLabel
    Left = 16
    Top = 32
    Width = 66
    Height = 13
    Caption = 'RadarCol File:'
  end
  object Button1: TButton
    Left = 640
    Top = 4
    Width = 59
    Height = 21
    Caption = '...'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Panel1: TPanel
    Left = 8
    Top = 56
    Width = 697
    Height = 441
    TabOrder = 1
    object PaintBox1: TPaintBox
      Left = 1
      Top = 1
      Width = 695
      Height = 439
      Align = alClient
      OnMouseDown = PaintBox1MouseDown
      OnMouseMove = PaintBox1MouseMove
      OnMouseUp = PaintBox1MouseUp
      OnPaint = PaintBox1Paint
    end
  end
  object Edit1: TEdit
    Left = 64
    Top = 508
    Width = 73
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 2
  end
  object Edit2: TEdit
    Left = 160
    Top = 508
    Width = 73
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 3
  end
  object TrackBar1: TTrackBar
    Left = 440
    Top = 504
    Width = 150
    Height = 29
    Min = 1
    Position = 5
    TabOrder = 4
  end
  object Button2: TButton
    Left = 600
    Top = 504
    Width = 99
    Height = 25
    Caption = 'Edit'
    TabOrder = 5
    OnClick = Button2Click
  end
  object Edit3: TEdit
    Left = 104
    Top = 4
    Width = 529
    Height = 21
    TabStop = False
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 6
  end
  object Button3: TButton
    Left = 640
    Top = 28
    Width = 59
    Height = 21
    Caption = '...'
    TabOrder = 7
    OnClick = Button3Click
  end
  object Edit4: TEdit
    Left = 104
    Top = 28
    Width = 529
    Height = 21
    TabStop = False
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 8
  end
  object XPManifest1: TXPManifest
    Left = 568
    Top = 464
  end
  object MainMenu1: TMainMenu
    Left = 168
    Top = 72
    object Map1: TMenuItem
      Caption = 'Map'
      object LoadMap1: TMenuItem
        Caption = 'Load Map'
        OnClick = Button1Click
      end
      object SaveMap1: TMenuItem
        Caption = 'Save Map'
        OnClick = SaveMap1Click
      end
      object LoadRadarcolmul1: TMenuItem
        Caption = 'Load Radarcol.mul'
        OnClick = Button3Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'mul'
    Filter = 'Map mul-files|*.mul|all files|*.*'
    Left = 280
    Top = 48
  end
  object OpenDialog2: TOpenDialog
    DefaultExt = 'mul'
    Filter = 'radarcol.mul|radarcol.mul|all files|*.*'
    Left = 312
    Top = 48
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'mul'
    Filter = 'Map0.mul|map0.mul|mul files|*.mul|all files|*.*'
    Left = 248
    Top = 48
  end
end
