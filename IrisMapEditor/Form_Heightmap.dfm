object EditForm: TEditForm
  Left = 311
  Top = 200
  Width = 640
  Height = 480
  Caption = 'Edit Heightmap'
  Color = clBtnFace
  Constraints.MinHeight = 480
  Constraints.MinWidth = 640
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 14
  object Panel1: TPanel
    Left = 0
    Top = 360
    Width = 632
    Height = 47
    Align = alBottom
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 40
      Height = 14
      Caption = 'Amount:'
    end
    object Label2: TLabel
      Left = 216
      Top = 16
      Width = 45
      Height = 14
      Caption = 'Pen Size:'
    end
    object TrackBar1: TTrackBar
      Left = 64
      Top = 8
      Width = 142
      Height = 29
      Max = 20
      Min = 1
      Position = 10
      TabOrder = 0
    end
    object TrackBar2: TTrackBar
      Left = 272
      Top = 8
      Width = 129
      Height = 29
      Min = 1
      Position = 1
      TabOrder = 1
    end
    object RadioButton1: TRadioButton
      Left = 496
      Top = 4
      Width = 73
      Height = 17
      Caption = 'Increase'
      Checked = True
      TabOrder = 2
      TabStop = True
    end
    object RadioButton2: TRadioButton
      Left = 496
      Top = 20
      Width = 73
      Height = 17
      Caption = 'Decrease'
      TabOrder = 3
    end
    object CheckBox3: TCheckBox
      Left = 408
      Top = 16
      Width = 81
      Height = 17
      Caption = 'Soft Edges'
      Checked = True
      State = cbChecked
      TabOrder = 4
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 407
    Width = 632
    Height = 19
    Panels = <
      item
        Alignment = taCenter
        Text = 'Control Points: 8 x 8'
        Width = 140
      end
      item
        Alignment = taCenter
        Text = 'Tiles: 64 x 64'
        Width = 140
      end
      item
        Width = 50
      end>
  end
  object Timer1: TTimer
    Interval = 50
    OnTimer = Timer1Timer
    Left = 488
    Top = 120
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'mul'
    FileName = 'map0.mul'
    Filter = 'MAP Files|*.mul'
    Left = 216
    Top = 144
  end
  object XPManifest1: TXPManifest
    Left = 552
    Top = 88
  end
  object MainMenu1: TMainMenu
    Left = 200
    Top = 104
    object Effects1: TMenuItem
      Caption = 'Effects'
      object AddNoise1: TMenuItem
        Caption = 'Add Noise'
        OnClick = Button2Click
      end
    end
    object Settings1: TMenuItem
      Caption = 'Settings'
      object ShowTileLines1: TMenuItem
        Caption = 'Show Tile Lines'
        Checked = True
        OnClick = ShowTileLines1Click
      end
      object Lighting1: TMenuItem
        Caption = 'Lighting'
        Checked = True
        OnClick = Lighting1Click
      end
    end
  end
end
