package com.poloz.noreturn.state;

import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import java.util.Stack;

/**
 * Created by Alex 'Poloz' Baranov on 22.11.2016.
 */
public class GameStateManager {
    private final Stack<AbstractState> states;

    public GameStateManager() {
        this.states = new Stack<AbstractState>();
    }

    public void push (AbstractState state){
        this.states.push(state);
    }

    public void pop (){
        this.states.pop().dispose();
    }

    public void set(AbstractState state) {
        this.pop();
        this.states.push(state);
    }

    public void update(float delta){
        this.states.peek().update(delta);
    }

    public void render(SpriteBatch batch){
        this.states.peek().render(batch);
    }
}
