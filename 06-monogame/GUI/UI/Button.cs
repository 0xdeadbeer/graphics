using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Vector2 = Microsoft.Xna.Framework.Vector2;

namespace GUI.UIElements;

public class ButtonData
{
    public Color bgColor;
    public Color fgColor;

    public Color? hoverBgColor; 
    public Color? hoverFgColor;

    public Color? pressedBgColor;
    public Color? pressedFgColor;

    public Color idleBgColor = Color.Black;
    public Color idleFgColor = Color.White;
    
    public bool isPressed = false;
    public bool isReleased = false;
    public bool isHovered = false;
    public bool isIdle = false;

    public Action<Button>? onClick;

    public string text = "";
    public Vector2 textPosition;
    public Vector2 textResolution;
    public Vector2 textScale;

    public ButtonData(Action<Button>? onClick)
    {
        this.onClick = onClick;
    }
}

public class Button : Drawable
{
    private DrawableData _drawableData;
    public ButtonData _buttonData;

    private SpriteFont buttonFont; 
    
    public Button(DrawableData data, ButtonData buttonData)
    {
        _drawableData = data;
        _buttonData = buttonData;
    }

    public override void Initialize()
    {
        buttonFont = Scene.content.Load<SpriteFont>("Fonts/default");
    }

    void onPressed()
    {
        if (_buttonData.isPressed)
            return;

        _buttonData.bgColor = _buttonData.pressedBgColor ?? _buttonData.idleBgColor;
        _buttonData.fgColor = _buttonData.pressedFgColor ?? _buttonData.idleFgColor;

        if (_buttonData.onClick is not null)
            _buttonData.onClick(this);
        
        _buttonData.isReleased = false;
        _buttonData.isPressed = true;
    }

    void onReleased()
    {
        if (!_buttonData.isPressed)
            return;

        _buttonData.bgColor = _buttonData.hoverBgColor ?? _buttonData.idleBgColor;
        _buttonData.fgColor = _buttonData.hoverFgColor ?? _buttonData.idleFgColor;
        
        _buttonData.isPressed = false;
        _buttonData.isReleased = true;
    }

    void onHovered()
    {
        if (_buttonData.isHovered)
            return;
        
        _buttonData.bgColor = _buttonData.hoverBgColor ?? _buttonData.idleBgColor;
        _buttonData.fgColor = _buttonData.hoverFgColor ?? _buttonData.idleFgColor;

        _buttonData.isHovered = true;
    }

    void onIdle()
    {
        _buttonData.bgColor = _buttonData.idleBgColor;
        _buttonData.fgColor = _buttonData.idleFgColor;
        
        _buttonData.isPressed = false;
        _buttonData.isReleased = false;
        _buttonData.isHovered = false;
    }

    public void buttonLogic()
    {
        var mstate = Mouse.GetState();
        Vector2 mousePosition = mstate.Position.ToVector2();

        bool outsideHorizontalBoundary = (mousePosition.X < _drawableData.position.X) || (mousePosition.X > (_drawableData.position.X + _drawableData.scale.X));
        bool outsideVerticalBoundary = (mousePosition.Y < _drawableData.position.Y) || (mousePosition.Y > (_drawableData.position.Y + _drawableData.scale.Y));

        if (outsideHorizontalBoundary || outsideVerticalBoundary)
        {
            onIdle();
            return;
        }

        if (mstate.LeftButton == ButtonState.Pressed)
        {
            onPressed();
            return;
        }

        if (mstate.LeftButton == ButtonState.Released)
        {
            onReleased();
        }
        
        onHovered();
    }

    public override void Update(GameTime gameTime)
    {
        buttonLogic();

        _buttonData.textResolution = buttonFont.MeasureString(_buttonData.text);
        
        // we want to center the text so 
        Vector2 buttonCenter = (_drawableData.position + (Vector2.One / 2.0f));
        Vector2 textHalfResolution = _buttonData.textResolution / 2.0f;

        _buttonData.textPosition = buttonCenter - textHalfResolution;
    } 
    
    public override void Draw(GameTime gameTime)
    {
        Scene.spriteBatch.Draw(
            _drawableData.texture, 
            _drawableData.position, 
            null, 
            Color.White, 
            _drawableData.rotation,
            Vector2.Zero,
            _drawableData.scale,
            SpriteEffects.None, 
            0f
        );
        
        Scene.spriteBatch.DrawString(
            buttonFont,
            _buttonData.text, 
            _buttonData.textPosition,
            _buttonData.fgColor, 
            _drawableData.rotation,
            Vector2.Zero,
            _drawableData.scale, 
            SpriteEffects.None, 
            0f, 
            false
        );
    }
}