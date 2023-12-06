using System;
using System.Data;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace GUI.UIElements;

public class DrawableData
{
    public Texture2D texture;
    public Vector2 position = Vector2.Zero;
    public Vector2 scale = Vector2.One;
    public float rotation = 0.0f;
    
    public DrawableData(Texture2D? texture)
    {
        if (texture is not null)
        {
            this.texture = texture;
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
    public abstract void Initialize(GameTime gameTime);
    public abstract void Update(GameTime gameTime);
    public abstract void Draw(GameTime gameTime);
}