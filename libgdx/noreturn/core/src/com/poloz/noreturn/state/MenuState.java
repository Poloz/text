package com.poloz.noreturn.state;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.poloz.noreturn.NoReturnGame;

/**
 * Created by A on 02.12.2016.
 */

public class MenuState extends AbstractState {

    private Texture img;
    private Texture img2;
    private Texture buttonPlay;
    private BitmapFont font;

    public MenuState(GameStateManager gsm) {
        super(gsm);
        img = new Texture("badlogic.jpg");
        img2 = new Texture("pine-001_texture.jpg");
        buttonPlay = new Texture("button1.png");
        font = new BitmapFont();

    }

    @Override
    public void update(float delta) {

    }

    @Override
    public void render(SpriteBatch batch) {
        batch.begin();
        batch.draw(img, 450, 0);
        batch.draw(img2, 0, 0);
        font.draw(batch, "Fukko press!", Gdx.graphics.getWidth() / 2 - 42, Gdx.graphics.getHeight() / 2 + 50);
        batch.draw(buttonPlay, Gdx.graphics.getWidth() / 2 - buttonPlay.getWidth() / 2, Gdx.graphics.getHeight() / 2 - buttonPlay.getHeight() / 2);
        batch.end();

    }

    @Override
    public void dispose() {
        img.dispose();
        img2.dispose();
        buttonPlay.dispose();
    }
}
