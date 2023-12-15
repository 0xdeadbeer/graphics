using System;
using System.Data;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace GUI.UIElements;

public class DrawableData
{
    public Texture2D texture;
    public Vector2 position = Vector2.Zero;
    private Vector2 scale_multiplier = Vector2.One;
    public Vector2 scale
    {
        get { return scale_multiplier * new Vector2(texture.Width, texture.Height); }
        set
        {
            scale_multiplier = value;
        }
    }
    
    public float rotation = 0.0f;
    
    public DrawableData(string? texture)
    {
        if (texture is not null)
        {
            this.texture = Scene.content.Load<Texture2D>(texture);
            return;
        }

        this.texture = Scene.content.Load<Texture2D>("button-default-texture");
    }
}

public enum DrawableType
{
    Container,
    Text,
    Button,
}

public abstract class Drawable
{
    public DrawableType drawableType;
    public abstract void Initialize();
    public abstract void Update(GameTime gameTime);
    public abstract void Draw(GameTime gameTime);
}