using System;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using GUI.UIElements;

namespace GUI;

public class Game1 : Game
{
    private GraphicsDeviceManager _graphics;
    private SpriteBatch _spriteBatch;
    private Scene mainScene; 

    public Game1()
    {
        _graphics = new GraphicsDeviceManager(this);
        Content.RootDirectory = "Content";
        IsMouseVisible = true;
    }

    protected override void Initialize()
    {
        _spriteBatch = new SpriteBatch(GraphicsDevice);

        Scene.graphics = _graphics;
        Scene.spriteBatch = _spriteBatch;
        Scene.content = Content;
        
        mainScene = new Scene();
        DrawableData drawableData = new DrawableData(null);
        drawableData.scale = new Vector2(120.0f, 40.0f);
        drawableData.position = new Vector2(100.0f, 100.0f);

        void buttonOnClick(Button obj)
        {
            Console.WriteLine("ACTION CALLED SUCCESSFULLY!");
        }

        ButtonData buttonData = new ButtonData(buttonOnClick);
        buttonData.bgColor = Color.Red;
        buttonData.fgColor = Color.White;
        buttonData.text = "Click me!";

        Button  mainButton = new Button(drawableData, buttonData);
        
        mainScene.drawables.Add(mainButton);

        mainScene.Initialize();
        base.Initialize();
    }

    protected override void LoadContent()
    {
    }

    protected override void Update(GameTime gameTime)
    {
        if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
            Exit();
        
        mainScene.Update(gameTime);

        base.Update(gameTime);
    }

    protected override void Draw(GameTime gameTime)
    {
        GraphicsDevice.Clear(Color.Black);

        mainScene.Draw(gameTime);

        base.Draw(gameTime);
    }
}
