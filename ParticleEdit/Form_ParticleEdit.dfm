object ParticleEditForm: TParticleEditForm
  Left = 38
  Top = 22
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Edit Effect'
  ClientHeight = 557
  ClientWidth = 714
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDefault
  OnActivate = FormActivate
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  OnMouseWheel = FormMouseWheel
  OnPaint = FormPaint
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object GroupBox1: TGroupBox
    Left = 453
    Top = 0
    Width = 259
    Height = 553
    Caption = 'Particle Definitions'
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 72
      Width = 72
      Height = 14
      Caption = 'Speed-Angles:'
    end
    object Label2: TLabel
      Left = 8
      Top = 96
      Width = 75
      Height = 14
      Caption = 'Speed-Amount:'
    end
    object Label5: TLabel
      Left = 160
      Top = 96
      Width = 13
      Height = 14
      Caption = '+/-'
    end
    object Label6: TLabel
      Left = 160
      Top = 168
      Width = 13
      Height = 14
      Caption = '+/-'
    end
    object Label7: TLabel
      Left = 8
      Top = 168
      Width = 105
      Height = 14
      Caption = 'Acceleration-Amount:'
    end
    object Label10: TLabel
      Left = 8
      Top = 144
      Width = 102
      Height = 14
      Caption = 'Acceleration-Angles:'
    end
    object Label11: TLabel
      Left = 8
      Top = 216
      Width = 40
      Height = 14
      Caption = 'Lifetime:'
    end
    object Label12: TLabel
      Left = 160
      Top = 216
      Width = 13
      Height = 14
      Caption = '+/-'
    end
    object Label13: TLabel
      Left = 8
      Top = 272
      Width = 66
      Height = 14
      Caption = 'Particle-Type:'
    end
    object Label14: TLabel
      Left = 8
      Top = 24
      Width = 57
      Height = 14
      Caption = 'Description:'
    end
    object Label3: TLabel
      Left = 8
      Top = 120
      Width = 93
      Height = 14
      Caption = 'Speed Cone Angle:'
    end
    object Label4: TLabel
      Left = 8
      Top = 192
      Width = 123
      Height = 14
      Caption = 'Acceleration Cone Angle:'
    end
    object Label22: TLabel
      Left = 8
      Top = 48
      Width = 89
      Height = 14
      Caption = 'Appear Time: (ms)'
    end
    object Label23: TLabel
      Left = 160
      Top = 48
      Width = 13
      Height = 14
      Caption = '+/-'
    end
    object Label8: TLabel
      Left = 225
      Top = 48
      Width = 14
      Height = 14
      Caption = 'ms'
    end
    object Label21: TLabel
      Left = 8
      Top = 296
      Width = 40
      Height = 14
      Caption = 'Amount:'
    end
    object Label16: TLabel
      Left = 157
      Top = 72
      Width = 4
      Height = 14
      Caption = #176
    end
    object Label17: TLabel
      Left = 221
      Top = 72
      Width = 4
      Height = 14
      Caption = #176
    end
    object Label18: TLabel
      Left = 157
      Top = 144
      Width = 4
      Height = 14
      Caption = #176
    end
    object Label24: TLabel
      Left = 221
      Top = 144
      Width = 4
      Height = 14
      Caption = #176
    end
    object Label25: TLabel
      Left = 221
      Top = 192
      Width = 4
      Height = 14
      Caption = #176
    end
    object Label26: TLabel
      Left = 221
      Top = 120
      Width = 4
      Height = 14
      Caption = #176
    end
    object Label27: TLabel
      Left = 225
      Top = 216
      Width = 6
      Height = 14
      Caption = 's'
    end
    object Label28: TLabel
      Left = 8
      Top = 344
      Width = 82
      Height = 14
      Caption = 'Transformations:'
    end
    object Label29: TLabel
      Left = 8
      Top = 240
      Width = 62
      Height = 14
      Caption = 'Particle Size:'
      OnDblClick = Button1Click
    end
    object Label30: TLabel
      Left = 160
      Top = 240
      Width = 13
      Height = 14
      Caption = '+/-'
    end
    object Label31: TLabel
      Left = 225
      Top = 240
      Width = 10
      Height = 14
      Caption = '%'
    end
    object Edit1: TEdit
      Left = 120
      Top = 68
      Width = 33
      Height = 22
      TabOrder = 3
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit3: TEdit
      Left = 184
      Top = 68
      Width = 33
      Height = 22
      TabOrder = 4
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit5: TEdit
      Left = 120
      Top = 92
      Width = 33
      Height = 22
      TabOrder = 5
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit6: TEdit
      Left = 184
      Top = 92
      Width = 33
      Height = 22
      TabOrder = 6
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit7: TEdit
      Left = 184
      Top = 164
      Width = 33
      Height = 22
      TabOrder = 11
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit8: TEdit
      Left = 120
      Top = 164
      Width = 33
      Height = 22
      TabOrder = 10
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit9: TEdit
      Left = 184
      Top = 140
      Width = 33
      Height = 22
      TabOrder = 9
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit12: TEdit
      Left = 120
      Top = 140
      Width = 33
      Height = 22
      TabOrder = 8
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit13: TEdit
      Left = 120
      Top = 212
      Width = 33
      Height = 22
      TabOrder = 13
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit14: TEdit
      Left = 184
      Top = 212
      Width = 33
      Height = 22
      TabOrder = 14
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object ComboBox1: TComboBox
      Left = 80
      Top = 268
      Width = 129
      Height = 22
      Style = csDropDownList
      ItemHeight = 14
      TabOrder = 17
      OnChange = Edit1Change
    end
    object Edit15: TEdit
      Left = 72
      Top = 20
      Width = 177
      Height = 22
      TabOrder = 0
      OnChange = Edit1Change
    end
    object Edit2: TEdit
      Left = 184
      Top = 116
      Width = 33
      Height = 22
      TabOrder = 7
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit4: TEdit
      Left = 184
      Top = 188
      Width = 33
      Height = 22
      TabOrder = 12
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit24: TEdit
      Left = 120
      Top = 44
      Width = 33
      Height = 22
      TabOrder = 1
      OnChange = Edit1Change
    end
    object Edit25: TEdit
      Left = 184
      Top = 44
      Width = 33
      Height = 22
      TabOrder = 2
      OnChange = Edit1Change
    end
    object Edit26: TEdit
      Left = 56
      Top = 292
      Width = 33
      Height = 22
      TabOrder = 19
      Text = '1'
      OnChange = Edit1Change
    end
    object UpDown: TUpDown
      Left = 89
      Top = 292
      Width = 16
      Height = 22
      Associate = Edit26
      Min = 1
      Max = 10000
      Position = 1
      TabOrder = 20
    end
    object Button8: TButton
      Left = 216
      Top = 268
      Width = 27
      Height = 21
      Caption = '...'
      TabOrder = 18
      OnClick = EditTypeDefinitions1Click
    end
    object CheckBox1: TCheckBox
      Left = 8
      Top = 320
      Width = 105
      Height = 17
      Caption = 'Transform Speed'
      TabOrder = 23
      OnClick = Edit1Change
    end
    object CheckBox2: TCheckBox
      Left = 128
      Top = 320
      Width = 121
      Height = 17
      Caption = 'Transform Accel.'
      TabOrder = 24
      OnClick = Edit1Change
    end
    object ListView1: TListView
      Left = 8
      Top = 360
      Width = 241
      Height = 153
      Columns = <
        item
          Caption = 'Emission Type'
          Width = 120
        end
        item
          AutoSize = True
          Caption = 'Parameters'
        end>
      HideSelection = False
      ReadOnly = True
      RowSelect = True
      TabOrder = 25
      ViewStyle = vsReport
      OnChange = ListView1Change
      OnDblClick = ListView1DblClick
    end
    object Button1: TButton
      Left = 8
      Top = 520
      Width = 49
      Height = 25
      Caption = 'new'
      TabOrder = 26
      OnClick = Button1Click
    end
    object Button5: TButton
      Left = 64
      Top = 520
      Width = 51
      Height = 25
      Caption = 'remove'
      TabOrder = 27
      OnClick = Button5Click
    end
    object Button6: TButton
      Left = 120
      Top = 520
      Width = 57
      Height = 25
      Caption = 'move up'
      TabOrder = 28
      OnClick = Button6Click
    end
    object Button7: TButton
      Left = 184
      Top = 520
      Width = 67
      Height = 25
      Caption = 'move down'
      TabOrder = 29
      OnClick = Button6Click
    end
    object CheckBox3: TCheckBox
      Left = 112
      Top = 296
      Width = 65
      Height = 17
      Caption = 'Fade out'
      TabOrder = 21
      OnClick = Edit1Change
    end
    object Edit10: TEdit
      Left = 120
      Top = 236
      Width = 33
      Height = 22
      TabOrder = 15
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit11: TEdit
      Left = 184
      Top = 236
      Width = 33
      Height = 22
      TabOrder = 16
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object CheckBox4: TCheckBox
      Left = 184
      Top = 296
      Width = 65
      Height = 17
      Caption = 'Fade in'
      TabOrder = 22
      OnClick = Edit1Change
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 264
    Width = 453
    Height = 289
    Caption = 'Effect Defintion'
    TabOrder = 1
    DesignSize = (
      453
      289)
    object Label15: TLabel
      Left = 8
      Top = 56
      Width = 57
      Height = 14
      Caption = 'Description:'
    end
    object Label19: TLabel
      Left = 208
      Top = 24
      Width = 36
      Height = 14
      Caption = 'Appear'
    end
    object Label20: TLabel
      Left = 344
      Top = 24
      Width = 14
      Height = 14
      Caption = 'ms'
    end
    object Label9: TLabel
      Left = 8
      Top = 104
      Width = 44
      Height = 14
      Caption = 'Particles:'
    end
    object Label32: TLabel
      Left = 8
      Top = 24
      Width = 30
      Height = 14
      Caption = 'Name:'
    end
    object Label33: TLabel
      Left = 288
      Top = 56
      Width = 96
      Height = 14
      Caption = 'particles on screen:'
    end
    object Label34: TLabel
      Left = 392
      Top = 56
      Width = 6
      Height = 14
      Caption = '0'
    end
    object Label35: TLabel
      Left = 8
      Top = 80
      Width = 207
      Height = 14
      Caption = 'Spelleffect ID (Reference from UO Spells) :'
    end
    object Edit16: TEdit
      Left = 72
      Top = 52
      Width = 209
      Height = 22
      TabOrder = 0
      OnChange = Edit16Change
    end
    object ListView2: TListView
      Left = 8
      Top = 120
      Width = 433
      Height = 137
      Anchors = [akLeft, akTop, akBottom]
      Columns = <
        item
          Caption = 'ID'
        end
        item
          AutoSize = True
          Caption = 'Description'
          MinWidth = 120
        end
        item
          Caption = 'Appear Time'
          Width = 120
        end
        item
          Caption = 'Amount'
          Width = 80
        end>
      HideSelection = False
      ReadOnly = True
      RowSelect = True
      TabOrder = 5
      ViewStyle = vsReport
      OnChange = ListView2Change
    end
    object RadioButton1: TRadioButton
      Left = 248
      Top = 16
      Width = 57
      Height = 17
      Caption = 'every'
      TabOrder = 1
      OnClick = Edit16Change
    end
    object RadioButton2: TRadioButton
      Left = 248
      Top = 32
      Width = 49
      Height = 17
      Caption = 'after'
      TabOrder = 2
      OnClick = Edit16Change
    end
    object Edit17: TEdit
      Left = 304
      Top = 20
      Width = 33
      Height = 22
      TabOrder = 3
      Text = '0'
      OnChange = Edit16Change
    end
    object Button4: TButton
      Left = 376
      Top = 20
      Width = 65
      Height = 21
      Caption = 'restart'
      TabOrder = 4
      OnClick = Button4Click
    end
    object Button2: TButton
      Left = 128
      Top = 262
      Width = 97
      Height = 21
      Caption = 'add particle'
      TabOrder = 6
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 336
      Top = 262
      Width = 97
      Height = 21
      Caption = 'delete particle'
      TabOrder = 7
      OnClick = Button3Click
    end
    object Edit18: TEdit
      Left = 72
      Top = 20
      Width = 129
      Height = 22
      TabOrder = 8
      OnChange = Edit16Change
    end
    object Button9: TButton
      Left = 232
      Top = 262
      Width = 97
      Height = 21
      Caption = 'dupe particle'
      TabOrder = 9
      OnClick = Button9Click
    end
    object Edit19: TEdit
      Left = 216
      Top = 76
      Width = 65
      Height = 22
      TabOrder = 10
      OnChange = Edit16Change
    end
  end
  object MainMenu1: TMainMenu
    object File1: TMenuItem
      Caption = 'File'
      object SaveDefinitionFile1: TMenuItem
        Caption = 'Save Definition File'
        OnClick = SaveDefinitionFile1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Close'
        OnClick = Exit1Click
      end
    end
    object Particles1: TMenuItem
      Caption = 'Particles'
      object EditTypeDefinitions1: TMenuItem
        Caption = 'Edit Type Definitions'
        OnClick = EditTypeDefinitions1Click
      end
    end
    object Settings1: TMenuItem
      Caption = 'Settings'
      object SetBackgroundColor1: TMenuItem
        Caption = 'Set Background Color...'
        OnClick = SetBackgroundColor1Click
      end
    end
  end
  object ApplicationEvents1: TApplicationEvents
    OnIdle = ApplicationEvents1Idle
    Left = 32
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'xml'
    Filter = 'Particle Definitions|*.xml'
    InitialDir = '.'
    Left = 64
  end
  object PopupMenu1: TPopupMenu
    Left = 328
    Top = 96
    object Rotation1: TMenuItem
      Caption = 'Rotation'
      OnClick = Rotation1Click
    end
    object Translation1: TMenuItem
      Caption = 'Translation'
      OnClick = Rotation1Click
    end
    object Circle1: TMenuItem
      Caption = 'Circle'
      OnClick = Rotation1Click
    end
    object Sphere1: TMenuItem
      Caption = 'Sphere'
      OnClick = Rotation1Click
    end
  end
  object ColorDialog1: TColorDialog
    Left = 312
    Top = 80
  end
end
