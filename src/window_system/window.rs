// Convert hex value to unicode 32 char
macro_rules! hex_to_char {
    ($a: expr) => {{
        char::from_u32($a as u32).unwrap()
    }};
}

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

pub struct Window {
    x: usize,
    y: usize,
    width: usize,
    height: usize,
    buffer: Vec<char>,
}

impl Window {
    pub fn new(start_x: usize, start_y: usize, w: usize, h: usize) -> Self {
        let mut buff_tmp = vec![' '; (w * h).into()];

        // Upper and lower horizontal lines
        for x in 1..(w - 1) {
            buff_tmp[x] = hex_to_char!(WindowBorder::BarHorizontal);
            buff_tmp[(h - 1) * w + x] = hex_to_char!(WindowBorder::BarHorizontal);
        }

        // Left and right vertical lines
        for y in 1..(h - 1) {
            buff_tmp[y * w] = hex_to_char!(WindowBorder::BarVertical);
            buff_tmp[y * w + (w - 1)] = hex_to_char!(WindowBorder::BarVertical);
        }

        // Corners
        buff_tmp[0] = hex_to_char!(WindowBorder::CrnTopLeft);
        buff_tmp[w - 1] = hex_to_char!(WindowBorder::CrnTopRight);
        buff_tmp[(h - 1) * w] = hex_to_char!(WindowBorder::CrnBottomLeft);
        buff_tmp[(h - 1) * w + (w - 1)] = hex_to_char!(WindowBorder::CrnBottomRight);


        Window { 
            x: start_x,
            y: start_y,
            width: w,
            height: h,
            buffer: buff_tmp,
        }
    }

    pub fn render(&self) {

    }

    pub fn get_child(&self) {

    }

    pub fn print(&self) {
        for c in self.buffer.iter() {
            print!("{}", c);
        }
    }
}