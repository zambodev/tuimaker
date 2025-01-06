use std::io::{stdout, Write};
use std::sync::{Arc, Mutex};
use crate::window_system::window::WindowRef;


// Structs ----------------------------------------------------------------------------------------
pub struct WindowManager {
    roots: Vec<WindowRef>
}

impl WindowManager {
    pub fn new() -> Self {
        WindowManager {
            roots: Vec::new()
        }
    }

    pub fn add_root(& mut self, win: WindowRef) {
        self.roots.push(win);
    }

    pub fn render(& mut self, root: WindowRef) {
        let window_deref = root.lock().unwrap();
    
        if !window_deref.is_root() {
            panic!("Trying to render a non root window!");
        }

        let (width, height) = window_deref.get_size();
        let buff: Arc<Mutex<Vec<char>>> = Arc::new(Mutex::new(vec![' '; (width * height).into()]));

        drop(window_deref);

        self.render_children(root.clone(), buff.clone(), width);
    
        let buff_deref = buff.lock().unwrap();

        for c in 0 .. buff_deref.len() {
            print!("{}", buff_deref[c]);
        }

        stdout().flush().unwrap();
    }

    fn render_children(& mut self, win: WindowRef, buff: Arc<Mutex<Vec<char>>>, father_width: usize) {
        let window_deref = win.lock().unwrap();
        let (x, y) = window_deref.get_cords();
        let (width, height) = window_deref.get_size();
        let w_buff = window_deref.get_buffer();
        let mut buff_mut = buff.lock().unwrap();

        for h in 0 .. height {
            for w in 0 .. width {
                buff_mut[(y + h) * father_width + (x + w)] = w_buff[h * width + w];
            }
        }

        drop(buff_mut);

        // Render next layer of childen
        for child_iter in window_deref.get_children() {
            self.render_children(child_iter.clone(), buff.clone(), width);
        }
    }
}
