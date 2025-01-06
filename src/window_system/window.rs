use std::sync::atomic::{AtomicI64, Ordering};
use std::sync::{Arc, Mutex};

// Macros -----------------------------------------------------------------------------------------
/// Convert hex value to unicode 32 char
macro_rules! hex_to_char {
    ($a: expr) => {{
        char::from_u32($a as u32).unwrap()
    }};
}

// Types ------------------------------------------------------------------------------------------
pub type WindowId = i64;
pub type WindowRef = Arc<Mutex<Window>>;
// Private variables ------------------------------------------------------------------------------
static NEXT_ID: AtomicI64 = AtomicI64::new(0);

// Enums ------------------------------------------------------------------------------------------
enum WindowBorder {
    BarHorizontal       = 0x2501,
    BarVertical         = 0x2503,
    CrnTopLeft          = 0x250F,
    CrnTopRight         = 0x2513,
    CrnBottomLeft       = 0x2517,
    CrnBottomRight      = 0x251B,
    Cross               = 0x254B,
    TLeft               = 0x2523,
    TRight              = 0x252B,
    TTop                = 0x2533,
    TBottom             = 0x253B,
    Space               = 0x0020
}

// Structs ----------------------------------------------------------------------------------------
pub struct Window {
    x: usize,
    y: usize,
    width: usize,
    height: usize,
    buffer: Vec<char>,
    id: WindowId,
    children: Vec<WindowRef>
}

impl<'a> Window {
    pub fn new(x: usize, y: usize, width: usize, height: usize) -> WindowRef {
        Arc::new(Mutex::new(Window {
            x,
            y,
            width,
            height,
            buffer: vec![' '; (width * height).into()],
            id: NEXT_ID.fetch_add(1, Ordering::Relaxed),
            children: Vec::new()
        }))
    }

    pub fn add_child(& mut self, win: WindowRef) {
        self.children.push(win);
    }

    pub fn draw(& mut self) {
        // Upper and lower horizontal lines
        for x in 1..(self.width - 1) {
            self.buffer[x] = hex_to_char!(WindowBorder::BarHorizontal);
            self.buffer[(self.height - 1) * self.width + x] = hex_to_char!(WindowBorder::BarHorizontal);
        }

        // Left and right vertical lines
        for y in 1..(self.height - 1) {
            self.buffer[y * self.width] = hex_to_char!(WindowBorder::BarVertical);
            self.buffer[y * self.width + (self.width - 1)] = hex_to_char!(WindowBorder::BarVertical);
        }

        // Corners
        self.buffer[0] = hex_to_char!(WindowBorder::CrnTopLeft);
        self.buffer[self.width - 1] = hex_to_char!(WindowBorder::CrnTopRight);
        self.buffer[(self.height - 1) * self.width] = hex_to_char!(WindowBorder::CrnBottomLeft);
        self.buffer[(self.height - 1) * self.width + (self.width - 1)] = hex_to_char!(WindowBorder::CrnBottomRight);
    }

    pub fn set_width(& mut self, width: usize) {
        self.width = width
    }

    pub fn set_height(& mut self, height: usize) {
        self.height = height
    }

    pub fn get_id(&self) -> WindowId {
        self.id
    }

    pub fn get_buffer(&'a self) -> &'a Vec<char> {
        &self.buffer
    }
}