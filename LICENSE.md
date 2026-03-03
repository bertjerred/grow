# grow License

## Primary License (ISC)

Copyright © 2026 Bert Jerred

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

---

## Third-Party Libraries & Credits

This project incorporates code from the following third-party libraries. The original licenses for these components are provided below and must be adhered to.

### DaisySP

- **License:** GNU Lesser General Public License (LGPL v2.1)
- **Usage:** Used for the reverb (`reverbsc.h`) and other DSP functions.
- **Compliance Note:** As `grow` uses DaisySP under the LGPL, it is dynamically linked. This ensures that end-users can replace the library with their own version, in compliance with the LGPL terms. The build system is configured to handle this.

### DPF (DISTRHO Plugin Framework)

- **License:** ISC License
- **Usage:** The core framework used to build `grow` as a cross-platform audio plugin.

### Jost* Font

- **License:** SIL Open Font License 1.1
- **Usage:** The user interface for `grow` utilizes the Jost* font, designed by indestructible type*. [2]
- **Attribution Note:** The SIL OFL requires that the copyright notice and license text are included with any distribution. Although the font is embedded in the source code, the copyright and license for Jost* should be made available to the end-user, for example in an "About" dialog within the plugin.
