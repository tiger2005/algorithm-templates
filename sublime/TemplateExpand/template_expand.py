import sublime
import sublime_plugin
import re

class TemplateExpandCommand(sublime_plugin.TextCommand):
  def run(self, edit, action):
    view = self.view
    if view.is_read_only():
      return
    lines = view.lines(sublime.Region(0, view.size()))
    try:
      prefix = sublime.load_settings("TemplaceExpand.sublime-settings").get('file-prefix')
      if prefix == None:
        return
    except Exception as r:
      print(r)
      return
    if action == "expand":

      overall_list = []
      modify_req = []

      for line in lines:
        cont = view.substr(line)
        mch = re.match(r'#include "lib/([\.\d\w\-/\s]+)"', cont)
        if mch:

          overall = []
          def dfs_file(name, plugin=True):
            if name in overall_list:
              return
            overall_list.append(name)
            plug = []
            fn = prefix + '/' + name
            cppF = None
            try:
              tmp = open(fn, "r", encoding = "utf-8")
              cppF = tmp.read().splitlines()
            except Exception as r:
              print(r)
              return

            if plugin:
              plug.append("// $START - " + name)
            else:
              plug.append("// #START - " + name)
            impl_start = False
            for l in cppF:
              _mch = re.match(r'#include "lib/([\.\d\w\-/\s]+)"', l)
              if _mch:
                dfs_file(_mch.group(1))
              else:
                if impl_start and len(l) != 0:
                    plug.append(l)
              if l == "// impl":
                impl_start = True

            if (plug[-1] == "#endif"):
              plug = plug[:-1]

            if plugin:
              plug.append("// $END - " + name)
            else:
              plug.append("// #END - " + name)

            for ele in plug:
              overall.append(ele)

          dfs_file(mch.group(1), False)
          modify_req.append((line, overall))

      modify_req.reverse()
      for line, cont in modify_req:
        view.replace(edit, line, '\n'.join(cont))
        cur_bracket = 0
        cur_line = -1
        line_index = view.rowcol(line.begin())[0]
        for c in cont:
          tmp = cur_bracket + c.count('{') - c.count('}')
          if tmp != 0 and cur_bracket == 0:
            if not c.startswith("#"):
              cur_line = line_index
          elif tmp == 0 and cur_bracket != 0 and cur_line != -1:
            view.fold(sublime.Region(view.text_point(cur_line, 1e9, clamp_column=True), view.text_point(line_index, 0, clamp_column=True)))
            cur_line = -1
          line_index += 1
          cur_bracket = tmp
      view.show(view.sel(), animate=False)

    elif action == "collapse":
      lines.reverse()
      in_bracket = False
      cur = ""
      cur_end = None
      is_plugin = False

      for line in lines:
        cont = view.substr(line)
        mch1 = re.match(r'// #START - ([\.\d\w\-/\s]+)', cont)
        mch2 = re.match(r'// #END - ([\.\d\w\-/\s]+)', cont)
        mch3 = re.match(r'// \$START - ([\.\d\w\-/\s]+)', cont)
        mch4 = re.match(r'// \$END - ([\.\d\w\-/\s]+)', cont)
        if mch2:
          if in_bracket == False:
            in_bracket = True
            cur = mch2.group(1)
            cur_end = line.end()
            is_plugin = False
        elif mch1:
          if in_bracket == True and cur == mch1.group(1):
            in_bracket = False
            view.unfold(sublime.Region(line.begin(), cur_end))
            view.replace(edit, sublime.Region(line.begin(), cur_end), "#include \"lib/" + cur + "\"")
        elif mch4:
          if in_bracket == False:
            in_bracket = True
            cur = mch4.group(1)
            cur_end = line.end()
            is_plugin = True
        elif mch3:
          if in_bracket == True and cur == mch3.group(1):
            in_bracket = False
            view.erase(edit, sublime.Region(line.begin(), cur_end + 1))

      view.show(view.sel(), animate=False)