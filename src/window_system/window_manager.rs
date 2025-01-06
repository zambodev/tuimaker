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

    pub fn render(& self, win: WindowRef) {
        let window_deref = win.lock().unwrap();
    
        for c in window_deref.get_buffer() {
            print!("{}", c);
        }
    }
}
